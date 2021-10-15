#pragma once
#include <TML/Types.h>
#include <TML/Utilities/Function_ptr.h>
#include <TML/Interface/Object.h>
#include <unordered_map>

namespace tml
{
    namespace Interface
    {
        class BaseComponent : public Object
        {
        public:
            struct Events
            {
                bool Click       = false;
                bool MouseEnter  = false;
                bool MouseExit   = false;
                bool MouseOver   = false;
                bool Drag        = false;
                bool MouseDown   = false;
                bool LostFocus   = false;
                bool GainedFocus = false;
            };
            struct StateFlag
            {
                bool Enabled     = true;
                bool Focused     = false;
                bool Resizeable  = false;
                bool Movable     = false;
            };
            enum SizePolicy { Fixed, Expand, Auto };
            using UIFunc = Function_ptr<void, BaseComponent*>;
        public:
            BaseComponent();
            using Object::Object;
            virtual ~BaseComponent();
            void Focus();
            void UnFocus();
            void Enable();
            void Disable();
            void ToggleEnabled();

            void SetOnClick(UIFunc function)        { m_onClickFunc        = function; }
            void SetOnMouseDown(UIFunc function)    { m_onMouseDownFunc    = function; }
            void SetOnHover(UIFunc function)        { m_onHoverFunc        = function; }
            void SetOnEnter(UIFunc function)        { m_onMouseEnterFunc   = function; }
            void SetOnExit(UIFunc function)         { m_onMouseExitFunc    = function; }
            void SetOnDrag(UIFunc function)         { m_onDragFunc         = function; }
            void SetOnUpdate(UIFunc function)       { m_onUpdate           = function; }
            void SetOnFocusGained(UIFunc function)  { m_onFocused          = function; }
            void SetOnFocusLost(UIFunc function)    { m_onFocusLost        = function; }

            void AddChild(BaseComponent* component, const std::string& name = "");
            const BaseComponent* FindChild(const std::string& name) const; // DANGER! Returns nullptr if not found.
            const BaseComponent* GetHead() const;
            virtual bool ContainsPoint(const Vector2& p);
            virtual void Update(float dt = (1.0f / 60.f));

            inline SizePolicy GetHorizontalSizePolicy() const { return m_hSizePolicy; }
            inline SizePolicy GetVerticalSizePolicy() const { return m_vSizePolicy; }

            void SetSizePolicy(SizePolicy horizontal, SizePolicy vertical)
            {
                m_hSizePolicy = horizontal;
                m_vSizePolicy = vertical;
            }

            void SetPrimaryColor(const Color& color)    { m_pColor = color;      }
            void SetSecondaryColor(const Color& color)  { m_sColor = color;      }
            void SetActiveColor(const Color& color)     { m_activeColor = color; }

        protected:
            void _Update(float dt);
            virtual void Draw() = 0;
            // Internal events
            virtual void OnMouseClick(const Vector2& mousePos);
            virtual void OnMouseDown(const Vector2& mousePos);
            virtual void OnMouseHover();
            virtual void OnMouseEnter();
            virtual void OnMouseExit();
            virtual void OnMouseDrag(const Vector2& mousePos);
            virtual void OnUpdate(float dt);
            virtual void OnFocused();
            virtual void OnFocusLost();

            UIFunc m_onClickFunc;
            UIFunc m_onMouseDownFunc;
            UIFunc m_onHoverFunc;
            UIFunc m_onMouseEnterFunc;
            UIFunc m_onMouseExitFunc;
            UIFunc m_onDragFunc;
            UIFunc m_onUpdate;
            UIFunc m_onFocused;
            UIFunc m_onFocusLost;

            Events m_eventStatus;
            StateFlag m_state;

            // Parent object checks these, and the children can check the parents m_mousePos or m_mouseClicked.
            // This is to avoid polling mouse position and click state on each node.
            Vector2 m_mousePos;
            bool m_mouseClicked = false;

            // Visual
            float m_animSpeed = 4.f;
            Color m_pColor, m_sColor, m_activeColor; // Primary and secondary color.
            SizePolicy m_hSizePolicy = Fixed; // Horizontal size policy.
            SizePolicy m_vSizePolicy = Fixed; // Vertical size policy.

            // Linked list
            BaseComponent* m_parent = nullptr;
            BaseComponent* m_child = nullptr;
            std::string m_name; // This is for finding a specific component from the list.
        };
    }
}