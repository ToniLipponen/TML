#pragma once
#include <cstdint>
#include <TML/Window/Input.h>
#include <TML/Window/Event.h>
#include <TML/System/Image.h>
#include <TML/System/Math/Vector2.h>
#include <TML/System/String.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Window
    {
    public:
        enum Settings : uint32_t
        {
            None                 = (0 <<  0), //!< No settings.
            Fullscreen           = (1 <<  0), //!< Make the window fullscreen. If you want to use the monitors resolution, use the UseMonitorResolution setting.
            Resizeable           = (1 <<  1), //!< Make the window resizeable.
            Maximized            = (1 <<  2), //!< Make the window maximized.
            VSync                = (1 <<  3), //!< Use vertical synchronization. (Note: Might not work on Microsoft Windows™ with non-fullscreen windows).
            Antialias            = (1 <<  4), //!< Enable antialiasing.
            NoTopBar             = (1 <<  5), //!< Make a window with no top bar / title bar.
            LimitAspect          = (1 <<  6), //!< Limit the window aspect ratio to the aspect ratio of width and height.
            LimitSize            = (1 <<  7), //!< Set the minimum size of the window to user supplied width and height.
            Hidden               = (1 <<  8), //!< Make the window hidden.
            Transparent          = (1 <<  9), //!< Make the window transparent. (Note: Might not be supported on all platforms).
            AlwaysOnTop          = (1 << 10), //!< Keep the window above other windows.
            UseMonitorResolution = (1 << 11), //!< Use the primary monitors size instead of user supplied width and height.
            NoClient             = (1 << 12), //!< Dont create an OpenGL context.
        };
    public:
        Window();
        Window(int32_t width, int32_t height, const String& title, uint32_t settings = None);
        Window(const Window &) = delete;
        Window(const Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        virtual ~Window() noexcept;
        virtual void Display() noexcept;

        bool Create(int32_t w, int32_t h, const String& title, uint32_t settings = None) noexcept;
        void Close() noexcept; //!< Destroys the window.
        bool IsOpen() const noexcept;
        
        const void* GetHandle() const noexcept;
        int32_t GetWidth() const noexcept;
        int32_t GetHeight() const noexcept;
        int32_t GetX() const noexcept;
        int32_t GetY() const noexcept;
        Vector2i GetSize() const noexcept;
        Vector2i GetPosition() const noexcept;

        bool PollEvents(Event& e) noexcept;
        bool WaitEvents(Event& e) noexcept;

        void SetPosition(int32_t x, int32_t y) noexcept;
        void SetSize(uint32_t width, uint32_t height) noexcept;
        void SetTitle(const String& title) noexcept;
        void SetIcon(const Image& image) const noexcept;
        void SetCursor(const Image& image) noexcept;
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
        void SetFullscreen(bool fullscreen, int32_t w = -1, int32_t h = -1) noexcept;
        void SetActive(bool active = true) const noexcept;
        void SetVisible(bool visible = true) const noexcept;
        void Show() const noexcept;
        void Hide() const noexcept;

    private:
        void HandleWindowEvents(const Event& e) noexcept;
        void SetCallbacks() noexcept;

    protected:
        Vector2i m_size, m_pos;
        void* m_handle = nullptr;
        void* m_cursor = nullptr;
        bool m_hasGLContext = false;
    };
}