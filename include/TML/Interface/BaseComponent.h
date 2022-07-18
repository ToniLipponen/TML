#pragma once
#include <TML/Graphics/RenderWindow.h>
#include <TML/System/String.h>
#include <TML/Export.h>
#include <deque>
#include <functional>
#include <vector>
#include <unordered_map>

#define TML_GUI_HANDLE_EVENT(eventName)                                      \
AddListener(eventName, [&](tml::Interface::BaseComponent*, const tml::Event&) \
{                                                                            \
    return true;                                                             \
})

#define TML_GUI_ON_EVENT(eventName, functionality)                                           \
AddListener(eventName, [&](tml::Interface::BaseComponent* COMPONENT, const tml::Event& EVENT) \
{                                                                                            \
    functionality                                                                            \
    return true;                                                                             \
})

namespace tml::Interface
{
    enum class ComponentAxis { Horizontal, Vertical };
    enum class SizePolicy { Fixed, Expand, Clamp };

    class Layer;

    class TML_API BaseComponent
    {
    public:
        struct StateFlag
        {
            bool MouseOver  = false;
            bool Enabled    = true;
            bool Focused    = false;
            bool Dragged    = false;
        };
        using EventCallback = std::function<bool(BaseComponent*, const Event&)>;

    public:
        BaseComponent() noexcept;
        BaseComponent(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept;
        BaseComponent(const BaseComponent&) = delete;
        BaseComponent& operator=(const BaseComponent&) noexcept = delete;
        virtual ~BaseComponent() noexcept;
        void Focus() noexcept;
        void UnFocus() noexcept;
        void Enable() noexcept;
        void Disable() noexcept;
        void ToggleEnabled() noexcept;
        StateFlag GetState() const noexcept;
        bool Focused() const noexcept;
        bool Enabled() const noexcept;

        /**
         * @brief Some valid listeners
         *
         * @Click When mouse has been pressed and then released over the component.
         * @MouseDown When mouse has been pressed over the component.
         * @MouseEnter When mouse enters the area of the component.
         * @MouseExit When mouse exits the are of the component.
         * @MouseMoved When mouse is moved.
         * @MouseScroll When mouse is scrolled.
         * @KeyPressed When keyboard key is pressed down.
         * @KeyReleased When keyboard key is released.
         * @TextEntered When text is entered.
         * @GainedFocus When the component gains focus.
         * @LostFocus When the component loses focus.
         * @Resized When the size of the component is changed.
         * @Moved When the position of the component is changed.
         * @ChildAdded When a child gets added to the component.
         * @ChildRemoved When a child gets removed from the component.
         * @WindowResized When a window returns a resize event.
         * @Drawn When the component is drawn
         * @Enabled When the component is enabled.
         * @Disabled When the component is disabled.
         * @Any When an event occurs.
         */
        void AddListener(const std::string& name, const EventCallback& callback) noexcept;
        void AddChild(BaseComponent* component, const std::string& id = "") noexcept;
        bool RemoveChild(const std::string& id) noexcept;
        bool RemoveChild(BaseComponent* component) noexcept;
        BaseComponent* FindComponent(const std::string& id) noexcept;    //!< DANGER! Returns nullptr if not found.
        BaseComponent* FindComponent(uint64_t) noexcept;                 //!< DANGER! Returns nullptr if not found.
        BaseComponent* GetParent() noexcept;                             //!< DANGER! Returns nullptr if the component doesn't have a parent.
        Layer* GetRoot() noexcept;
        uint64_t GetHash() const noexcept;
        const std::string& GetID() const noexcept;
        virtual bool ContainsPoint(const Vector2i& p);
        SizePolicy GetHorizontalSizePolicy() const noexcept;
        SizePolicy GetVerticalSizePolicy() const noexcept;
        void SetSizePolicy(SizePolicy horizontal, SizePolicy vertical) noexcept;
        void SetPrimaryColor(const Color& color) noexcept;
        void SetSecondaryColor(const Color& color) noexcept;
        void SetActiveColor(const Color& color) noexcept;
        virtual void SetTextColor(const Color& color) noexcept;
        void SetRoundness(float radius) noexcept;
        void ForEachChild(const std::function<bool(BaseComponent* c)>& function) noexcept;
        void SetPosition(const Vector2i& position) noexcept;
        void SetPosition(int32_t x, int32_t y) noexcept;
        void SetSize(const Vector2i& size) noexcept;
        void SetSize(uint32_t width, uint32_t height) noexcept;
        Vector2i GetOriginalSize() const noexcept;
        Vector2f GetSize() const noexcept;
        Vector2f GetPosition() const noexcept;

        friend Layer;
    public:
        [[maybe_unused]] static void SetGlobalAnimationSpeed(float speed) noexcept;
        [[maybe_unused]] static void SetGlobalDefaultPrimaryColor(const Color& color) noexcept;
        [[maybe_unused]] static void SetGlobalDefaultSecondaryColor(const Color& color) noexcept;
        [[maybe_unused]] static void SetGlobalDefaultActiveColor(const Color& color) noexcept;
        [[maybe_unused]] static void SetGlobalDefaultTextColor(const Color& color) noexcept;

    protected:
        bool CallUIFunc(const std::string& name, const Event& event) noexcept;
        bool HandleEvent(const Event& event) noexcept;
        virtual void pDraw(RenderTarget& renderer) noexcept = 0;

        Vector2f m_pos;
        Vector2f m_size;
        Vector2f m_originalSize; //!< Layouts might resize a component, so the original size needs to be saved.
        Vector2f m_minimumSize;  //!< Minimum size of the component.

        Color m_pColor;
        Color m_sColor;
        Color m_activeColor;
        Color m_textColor;
        float m_roundness = 0;

        SizePolicy m_hSizePolicy = SizePolicy::Fixed; // Horizontal size policy.
        SizePolicy m_vSizePolicy = SizePolicy::Fixed; // Vertical size policy.

        uint64_t m_hash = 0;
        std::string m_id;
        std::vector<std::unique_ptr<BaseComponent>> m_children;
        std::unordered_map<std::string, std::vector<EventCallback>> m_listeners;
        BaseComponent* m_parent;
        Layer* m_root;
        StateFlag m_state;

        /** Might be used in the future. */
        [[maybe_unused]] static float s_scale;
        static float s_animationSpeed;
        static Color s_defaultPrimaryColor;
        static Color s_defaultSecondaryColor;
        static Color s_defaultActiveColor;
        static Color s_defaultTextColor;
    };
}