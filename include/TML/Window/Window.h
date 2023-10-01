#pragma once
#include <TML/Window/WindowSettings.h>
#include <TML/Window/Input.h>
#include <TML/Window/Event.h>
#include <TML/Window/Cursor.h>
#include <TML/Window/Monitor.h>
#include <TML/System/EventHandler.h>
#include <TML/System/Image.h>
#include <TML/System/Math/Vector2.h>
#include <TML/System/String.h>
#include <TML/Export.h>
#include <optional>
#include <cstdint>

namespace tml
{
    class TML_API Window
    {
    public:
        Window() noexcept;
        Window(int32_t width, int32_t height, const String& title, int32_t flags = 0) noexcept;
        explicit Window(const WindowSettings& settings) noexcept;
        Window(const Window &) = delete;
        Window(const Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        virtual ~Window() noexcept;
        virtual void Display() noexcept;

        bool Create(int32_t w, int32_t h, const String& title, int32_t flags = 0) noexcept;
        bool Create(const WindowSettings& settings) noexcept;
        void Close() noexcept;

        [[nodiscard]] bool IsOpen(bool waitForEvent = false) const noexcept;
		[[nodiscard]] bool IsMaximized() const noexcept;
        [[nodiscard]] const void* GetHandle() const noexcept;
        [[nodiscard]] int32_t GetWidth() const noexcept;
        [[nodiscard]] int32_t GetHeight() const noexcept;
        [[nodiscard]] int32_t GetX() const noexcept;
        [[nodiscard]] int32_t GetY() const noexcept;
        [[nodiscard]] Vector2i GetSize() const noexcept;
        [[nodiscard]] Vector2i GetPosition() const noexcept;
        [[nodiscard]] const WindowSettings& GetSettings() const noexcept;
		[[nodiscard]] Monitor GetMonitor() const noexcept;
		[[nodiscard]] Vector2f GetContentScale() const noexcept;

        void SetPosition(int32_t x, int32_t y) noexcept;
        void SetSize(uint32_t width, uint32_t height) noexcept;
        void SetTitle(const String& title) noexcept;
        void SetIcon(const Image& image) const noexcept;
        void SetCursor(const Cursor& cursor) noexcept;
        void ResetCursor() noexcept;
        void HideCursor() noexcept;
        void ShowCursor() noexcept;

        void SetAspectRatio(int32_t x, int32_t y) noexcept;
        void SetAspectRatio(const Vector2i& ratio) noexcept;
        void SetSizeLimits(int32_t minWidth, int32_t minHeight, int32_t maxWidth, int32_t maxHeight) noexcept;
        void SetSizeLimits(const Vector2i& min, const Vector2i& max) noexcept;

        void Minimize() const noexcept;
        void Maximize() const noexcept;
        void Restore() const noexcept;
        void SetFullscreen(bool fullscreen, const std::optional<Monitor>& monitor = std::nullopt) noexcept;
        void SetActive(bool active = true) const noexcept;
        void SetVisible(bool visible = true) const noexcept;
        void Show() const noexcept;
        void Hide() const noexcept;

        EventHandler<Window, MouseButtonEvent> OnMouseClicked;
        EventHandler<Window, MoveEvent> OnMoved;
        EventHandler<Window, ResizeEvent> OnResized;
        EventHandler<Window, Event> OnMinimized;
        EventHandler<Window, Event> OnRestored;
        EventHandler<Window, Event> OnMaximized;

        EventHandler<Window, Event> OnMouseEnter;
        EventHandler<Window, Event> OnMouseExit;
        EventHandler<Window, Event> OnClosed;
        EventHandler<Window, Event> OnLostFocus;
        EventHandler<Window, Event> OnGainedFocus;


        EventHandler<Window, KeyEvent> OnKeyPressed;
        EventHandler<Window, MoveEvent> OnMouseMoved;
        EventHandler<Window, TextEvent> OnTextEntered;
        EventHandler<Window, MouseButtonEvent> OnMouseDown;
        EventHandler<Window, MouseScrollEvent> OnMouseScrolled;
        EventHandler<Window, MouseDragEvent> OnMouseDragged;
        EventHandler<Window, GamepadEvent> OnGamepad;
        EventHandler<Window, FileDropEvent> OnFilesDropped;

    private:
        void SetCallbacks() noexcept;

    protected:
        WindowSettings m_settings;
        Vector2i m_size, m_pos, m_mouseDownPos;
        bool m_mouseDown = false;
        bool m_dragging = false; // if set, don't send click event
        Mouse::Button m_buttonDown;
        float m_minDragDistance = 10;

        void* m_handle = nullptr;
        bool m_hasGLContext = false;
    };
}