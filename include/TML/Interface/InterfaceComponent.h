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
                Click       = (1 << 1),
                MouseEnter  = (1 << 2),
                MouseExit   = (1 << 3),
                MouseOver   = (1 << 4),
                Drag        = (1 << 5)
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
            const BaseComponent* FindChild(const std::string& name) const; // DANGER! Returns nullptr if not found.
            virtual bool ContainsPoint(const Vector2& p);
            virtual void Update(float dt = (1.0f / 60.f));

            Vector2 GetBounds();
            static BaseComponent* ActiveComponent;
        protected:
            void _Update(float dt);
            virtual void Draw() = 0;
            // Internal events
            virtual void OnMouseClick(const Vector2& mousePos);
            virtual void OnMouseHover();
            virtual void OnMouseEnter();
            virtual void OnMouseExit();
            virtual void OnUpdate(float dt);

            UIFunc m_onClickFunc;
            UIFunc m_onHoverFunc;
            UIFunc m_onMouseEnterFunc;
            UIFunc m_onMouseExitFunc;
            UIFunc m_onDragFunc;
            UIFunc m_onUpdate;

            ui32 m_eventStatus = 0;
            ui32 m_state = 0;

            Vector2 m_absPos = {0,0},   // Actual position
            m_absSize        = {0,0},   // Actual size
            m_relPos         = {0,0},   // Position relative to the parent
            m_relSize        = {0,0};   // Size relative to the parent

            // Parent object checks these, and the children can check the parents m_mousePos or m_mouseClicked.
            // This is to avoid polling mouse position and click state on each node.
            Vector2 m_mousePos;
            bool m_mouseClicked = false;

            // Visual
            float m_animSpeed = 2.f;
            Color m_pColor, m_sColor, m_activeColor; // Primary and secondary color.

            /**************************************************/
            BaseComponent* m_parent = nullptr;
            BaseComponent* m_child = nullptr;
            std::string m_name; // This is for finding a specific component from the list.
        };
    }
}