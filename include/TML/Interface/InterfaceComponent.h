#pragma once
#include <TML/Types.h>
#include <TML/Utilities/Function_ptr.h>
#include <unordered_map>

namespace tml
{
    namespace Interface
    {
        class BaseComponent
        {
        public:
            enum Event : ui32
            {
                Click       = (1 << 2),
                MouseEnter  = (1 << 3),
                MouseExit   = (1 << 4),
                MouseOver   = (1 << 5),
                Drag        = (1 << 6)
            };
            enum StateFlag : ui32
            {
                Enabled     = (1 << 1),
                Focused     = (1 << 2),
                Resizeable  = (1 << 3),
                Movable     = (1 << 4)
            };
            using UIFunc = Function_ptr<void, BaseComponent*>;
        public:
            BaseComponent();
            virtual ~BaseComponent();
            void Focus();
            void UnFocus();
            void Enable();
            void Disable();
            void SetOnClick(UIFunc function) { m_onClickFunc        = function; }
            void SetOnHover(UIFunc function) { m_onHoverFunc        = function; }
            void SetOnEnter(UIFunc function) { m_onMouseEnterFunc   = function; }
            void SetOnExit(UIFunc function)  { m_onMouseExitFunc    = function; }
            void SetOnDrag(UIFunc function)  { m_onDragFunc         = function; }
            void SetOnUpdate(UIFunc function){ m_onUpdate           = function; }
            void AddChild(BaseComponent* component, const std::string& name = "");
            virtual bool ContainsPoint(const Vector2& p) = 0;
            void Update(BaseComponent* parent);
        protected:
            virtual void Draw() = 0;
//            virtual Vector2 GetChildPosition(BaseComponent* child);
            UIFunc m_onClickFunc;
            UIFunc m_onHoverFunc;
            UIFunc m_onMouseEnterFunc;
            UIFunc m_onMouseExitFunc;
            UIFunc m_onDragFunc;
            UIFunc m_onUpdate;

            ui32 m_eventStatus = 0;
            ui32 m_state = 0;

            Vector2 m_absPos = {0,0},   // Actual position
            m_absSize = {0, 0},         // Actual size
            m_relPos = {0,0},           // Position relative to the parent
            m_relSize = {0,0};          // Size relative to the parent

            // Parent object checks these, and the children can check the parents m_mousePos or m_mouseClicked.
            // This is to avoid polling mouse position and click state on each child object.
            Vector2 m_mousePos;
            bool m_mouseClicked = false;

            Color m_pColor, m_sColor; // Primary and secondary color.
            std::unordered_map<std::string, BaseComponent*> m_children;
        };
    }
}