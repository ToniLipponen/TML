#pragma once
#include <TML/Types.h>
#include <TML/Window/Input.h>
#include <TML/Window/Event.h>
#include <TML/System/Image.h>

namespace tml
{
    class Window
    {
    public:
        enum Settings : ui32
        {
            None                 = (0 <<  0), //!< No settings.
            Fullscreen           = (1 <<  0), //!< Make the window fullscreen. If you want to use the monitors resolution, use the UseMonitorResolution setting.
            Resizeable           = (1 <<  1), //!< Make the window resizeable.
            Maximized            = (1 <<  2), //!< Make the window maximized.
            VSync                = (1 <<  3), //!< Use vertical synchronization. (Note: Might not work on windows with non fullscreen windows).
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
        Window(i32 width, i32 height, const std::string& title, ui32 settings = None, const Window* shared = nullptr);
        Window(const Window &) = delete;
        Window(const Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        virtual ~Window();
        virtual void Display();

        bool Create(i32 w, i32 h, const std::string& title, ui32 settings = None, const Window* shared = nullptr) noexcept;
        void Close() noexcept; /// Destroys the window.
        bool IsOpen() const noexcept;
        
        const void* GetHandle() const noexcept;
        i32 GetWidth() const noexcept;
        i32 GetHeight() const noexcept;
        i32 GetX() const noexcept;
        i32 GetY() const noexcept;
        Vector2i GetSize() const noexcept;
        Vector2i GetPosition() const noexcept;

        bool PollEvents(Event& e) const noexcept;
        bool WaitEvents(Event& e) const noexcept;

        void SetPosition(i32 x, i32 y) noexcept;
        void SetSize(ui32 width, ui32 height) noexcept;
        void SetTitle(const std::string& title) noexcept;
        void SetIcon(const Image& image) const noexcept;
        void SetCursor(const Image& image) noexcept;
        void ResetCursor() noexcept;
        void HideCursor() noexcept;
        void ShowCursor() noexcept;

        void SetAspectRatio(i32 x, i32 y) noexcept;
        void SetAspectRatio(const Vector2i& ratio) noexcept;
        void SetSizeLimits(i32 minWidth, i32 minHeight, i32 maxWidth, i32 maxHeight) noexcept;
        void SetSizeLimits(const Vector2i& min, const Vector2i& max) noexcept;

        void Minimize() const noexcept;
        void Maximize() const noexcept;
        void Restore() const noexcept;
        void SetFullscreen(bool fullscreen, i32 w = -1, i32 h = -1) noexcept;
        void SetActive() const noexcept;
        void SetVisible(bool visible = true) const noexcept;
        void Show() const noexcept;
        void Hide() const noexcept;

    protected:
        void SetCallbacks();
        void* m_handle = nullptr;
        void* m_cursor = nullptr;
        static bool s_glfwInitialized;
    };
}