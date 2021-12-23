#pragma once
#include <TML/Utilities/Types.h>
#include <TML/Interface/Object.h>
#include <TML/IO/Event.h>
#include <TML/IO/Input.h>

#include <deque>
#include <functional>
#include <vector>
#include <unordered_map>

namespace tml
{
    namespace Interface
    {
        enum ComponentAxis { Horizontal, Vertical }; // For components that can be either horizontal or vertical, for example sliders and scrollbars.

        class BaseComponent : public Object
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
            BaseComponent(BaseComponent* parent);
            using Object::Object;
            virtual ~BaseComponent();
            void Focus();
            void UnFocus();
            void Enable();
            void Disable();
            void ToggleEnabled();
            StateFlag GetState() const noexcept { return m_state; }
            bool Focused() const { return m_state.Focused; }
            bool Enabled() const;

            /*
             * Valid listeners:
             * Click
             * MouseDown
             * MouseOver
             * MouseEnter
             * MouseExit
             * MouseMoved
             * MouseScroll
             * KeyPressed
             * KeyReleased
             * TextEntered
             * Update
             * Focused
             * LostFocus
             * Event
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
            virtual void Update(Event event);

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
            void ProcessEvents(Event& event);

        protected:
            bool CallUIFunc(const std::string& name, Event& event);
            virtual void Draw() = 0;
            virtual void OnUpdate(double dt){};

            Color m_pColor;
            Color m_sColor;
            Color m_activeColor;

            SizePolicy m_hSizePolicy = Clamp; // Horizontal size policy.
            SizePolicy m_vSizePolicy = Fixed; // Vertical size policy.

            ui64 m_hash = 0; // hash of m_id
            std::string m_id;
            std::vector<BaseComponent*> m_children;
            std::unordered_map<std::string, std::vector<UIFunc>> m_listeners;
            BaseComponent* m_parent;
            StateFlag m_state;

            static std::hash<std::string> s_hash;
            static std::vector<BaseComponent*> s_processStack;
        };
    }
}