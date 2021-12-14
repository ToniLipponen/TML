#pragma once
#include <TML/Utilities/Types.h>
#include <TML/Interface/Object.h>
#include <deque>
#include <functional>
#include <vector>

namespace tml
{
    namespace Interface
    {
        enum ComponentAxis { Horizontal, Vertical}; // For components that can be either horizontal or vertical, for example sliders and scrollbars.

        class BaseComponent : public Object
        {
        public:
            struct Events
            {
                bool Click          = false;
                bool MouseEnter     = false;
                bool MouseExit      = false;
                bool MouseOver      = false;
                bool Drag           = false;
                bool MouseDown      = false;
                bool LostFocus      = false;
                bool GainedFocus    = false;
            };
            struct StateFlag
            {
                bool Enabled    = true;
                bool Focused    = false;
                bool Resizeable = false;
                bool Movable    = false;
                bool Raise      = false;
            };
            enum SizePolicy { Fixed, Expand, Clamp };
            using UIFunc = std::function<void(BaseComponent*)>;

        public:
            BaseComponent();
            BaseComponent(BaseComponent* parent);
            using Object::Object;
            virtual ~BaseComponent();
            void Focus();
            void UnFocus();
            void Enable();
            void Disable();
            void ToggleEnabled();
            void CanBeRaised(bool value); // Set this to true if you want this component to be raised above others if gained focus / clicked.

            bool Focused() const { return m_state.Focused; }
            bool Enabled() const;

            void SetOnClick(UIFunc function)        { m_onClickFunc         = function; }
            void SetOnMouseDown(UIFunc function)    { m_onMouseDownFunc     = function; }
            void SetOnHover(UIFunc function)        { m_onHoverFunc         = function; }
            void SetOnEnter(UIFunc function)        { m_onMouseEnterFunc    = function; }
            void SetOnExit(UIFunc function)         { m_onMouseExitFunc     = function; }
            void SetOnDrag(UIFunc function)         { m_onDragFunc          = function; }
            void SetOnUpdate(UIFunc function)       { m_onUpdate            = function; }
            void SetOnFocusGained(UIFunc function)  { m_onFocused           = function; }
            void SetOnFocusLost(UIFunc function)    { m_onFocusLost         = function; }

            void AddChild(BaseComponent* component, const std::string& name = "");
            BaseComponent* FindComponent(const std::string& name); // DANGER! Returns nullptr if not found.
            BaseComponent* FindComponent(unsigned long hash); // DANGER! Returns nullptr if not found.

            BaseComponent* GetParent(); // DANGER! Returns nullptr if the component doesn't have a parent.
            BaseComponent* GetRoot();
            virtual bool ContainsPoint(const Vector2i& p);
            virtual void Update();

            inline SizePolicy GetHorizontalSizePolicy() const { return m_hSizePolicy; }
            inline SizePolicy GetVerticalSizePolicy() const { return m_vSizePolicy; }

            void SetSizePolicy(SizePolicy horizontal, SizePolicy vertical)
            {
                m_hSizePolicy = horizontal;
                m_vSizePolicy = vertical;
            }

            void SetPrimaryColor(const Color& color) { m_pColor = color; }
            void SetSecondaryColor(const Color& color) { m_sColor = color; }
            void SetActiveColor(const Color& color) { m_activeColor = color; }
            void Raise();

        private:
            void ProcessEvents(const Vector2i& mp, bool& mouseDown, Events& evnt);

        protected:
            void p_Update(double dt, const Vector2i& mp);
            virtual void Draw() = 0;
            // Internal events
            virtual void OnMouseClick(const Vector2i& mousePos);
            virtual void OnMouseDown(const Vector2i& mousePos);
            virtual void OnMouseHover(const Vector2i& mousePos);
            virtual void OnMouseEnter();
            virtual void OnMouseExit();
            virtual void OnMouseDrag(const Vector2i& mousePos);
            virtual void OnUpdate(double dt);
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

            Events m_event;
            StateFlag m_state;

            // Visual
            float m_animSpeed = 4.f;
            Color m_pColor, m_sColor, m_activeColor; // Primary and secondary color.
            SizePolicy m_hSizePolicy = Clamp; // Horizontal size policy.
            SizePolicy m_vSizePolicy = Fixed; // Vertical size policy.

            std::deque<std::pair<unsigned long, BaseComponent*>> m_children;
            BaseComponent* m_parent;

            static std::hash<std::string> s_hash;
            static std::vector<BaseComponent*> s_processStack;
            static std::vector<BaseComponent*> s_renderStack;
        };
    }
}