#pragma once

#include <TML/Graphics/RenderWindow.h>

#include <deque>
#include <functional>
#include <vector>
#include <unordered_map>

namespace tml
{
    namespace Interface
    {
        enum ComponentAxis { Horizontal, Vertical }; // For components that can be either horizontal or vertical, for example sliders and scrollbars.

        class BaseComponent
        {
        public:
            struct StateFlag
            {
                char MouseDown   = -1;
                bool MouseOver  = false;
                bool Enabled    = true;
                bool Focused    = false;
                bool Dragged    = false;
                bool Resizeable = false;
                bool Movable    = false;
                bool Raise      = false;
            };
            enum SizePolicy { Fixed, Expand, Clamp };
            using UIFunc = std::function<void(BaseComponent*, Event&)>;

        public:
            BaseComponent();
            BaseComponent(i32 x, i32 y, ui32 w, ui32 h);
            virtual ~BaseComponent();
            void Focus();
            void UnFocus();
            void Enable();
            void Disable();
            void ToggleEnabled();
            StateFlag GetState() const noexcept { return m_state; }
            bool Focused() const { return m_state.Focused; }
            bool Enabled() const;

            /**
             * @Click When mouse has been pressed and then released over the component.
             * @MouseDown When mouse has been pressed over the component.
             * @MouseEnter When mouse enters the area of the component.
             * @MouseExit When mouse exits the are of the component.
             * @MouseMoved When mouse is moved.
             * @MouseScroll When mouse is scrolled.
             * @KeyPressed When keyboard key is pressed down.
             * @KeyReleased When keyboard key is released.
             * @TextEntered When text is entered.
             * @Update Called when the component is updated.
             * @GainedFocus When the component gains focus.
             * @LostFocus When the component loses focus.
             * @Resized When the size of the component is changed.
             * @Moved When the position of the component is changed.
             * @Any When an event occurs.
             */
            void AddListener(const std::string& name, UIFunc callback);
            void AddChild(BaseComponent* component, const std::string& id = "");

            BaseComponent* FindComponent(const std::string& id); // DANGER! Returns nullptr if not found.
            BaseComponent* FindComponent(ui64); // DANGER! Returns nullptr if not found.

            BaseComponent* GetParent(); // DANGER! Returns nullptr if the component doesn't have a parent.
            BaseComponent* GetRoot();

            constexpr inline ui64 GetHash() const noexcept { return m_hash; }
            constexpr inline const std::string& GetID() const noexcept { return m_id; }
            virtual bool ContainsPoint(const Vector2i& p);
            virtual void Update(Event& event, RenderWindow& renderWindow);

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

            void ForEachChild(const std::function<void(BaseComponent* c)>& function)
            {
                for(auto* i : m_children)
                    function(i);
            }

            void SetPosition(const Vector2i& position);
            void SetSize(const Vector2i& size);
            inline constexpr Vector2i GetPosition() const { return m_pos; }
            inline constexpr Vector2i GetSize() const { return m_size; }


        protected:
            void ClearFocused();
            void AddToProcessStack(BaseComponent* component);
            void RemoveFromProcessStack(BaseComponent* component);
            bool CallUIFunc(const std::string& name, Event& event);
            void ProcessEvents(Event& event, double dt);
            virtual void Draw(RenderWindow& renderWindow) = 0;;

            Vector2i m_pos;
            Vector2i m_size;
            Vector2i m_originalSize; // Layouts might resize a component, so the original size needs to be saved.
            Color m_pColor;
            Color m_sColor;
            Color m_activeColor;

            SizePolicy m_hSizePolicy = Fixed; // Horizontal size policy.
            SizePolicy m_vSizePolicy = Fixed; // Vertical size policy.

            ui64 m_hash = 0; // hash of m_id
            std::string m_id;
            std::vector<BaseComponent*> m_children;
            std::unordered_map<std::string, std::vector<UIFunc>> m_listeners;
            std::vector<BaseComponent*> m_processStack;
            BaseComponent* m_parent;
            StateFlag m_state;

            static std::hash<std::string> s_hash;
        };
    }
}