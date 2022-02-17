#pragma once
#include <TML/Types.h>
#include <TML/Window/Input.h>
#include <TML/Window/Event.h>

namespace tml
{
    class Window
    {
    public:
        enum Settings : ui32
        {
            None        = (0 << 0),
            Fullscreen  = (1 << 0),
            Resizeable  = (1 << 1),
            Maximized   = (1 << 2),
            VSync       = (1 << 3),
            Antialias   = (1 << 4),
            NoTopBar    = (1 << 5),
            LimitAspect = (1 << 6),
            Hidden      = (1 << 7)
        };
    public:
        Window(i32 width, i32 height, const std::string& title, ui32 settings = None);
        Window(const Window &) = delete;
        Window(const Window&&) = delete;
        Window &operator=(const Window&) = delete;
        Window &operator=(Window&&) = delete;

        virtual ~Window();
        virtual void Display();
        void Close() noexcept;
        bool ShouldClose() const noexcept;
        
        void *GetHandle() noexcept;
        i32 GetWidth() const noexcept;
        i32 GetHeight() const noexcept;
        i32 GetX() const noexcept;
        i32 GetY() const noexcept;
        Vector2i GetSize() const noexcept;
        Vector2i GetPosition() const noexcept;

        Event PollEvents() noexcept;
        Event WaitEvents() noexcept;

        void SetSize(ui32 width, ui32 height) noexcept;
        void SetTitle(const std::string& title) noexcept;

        void SetAspectRatio(i32 x, i32 y) noexcept;
        void SetAspectRatio(const Vector2i& ratio) noexcept;
        void SetSizeLimits(i32 minWidth, i32 minHeight, i32 maxWidth, i32 maxHeight) noexcept;
        void SetSizeLimits(const Vector2i& min, const Vector2i& max) noexcept;

        void Maximize() noexcept;
        void SetFullscreen(bool fullscreen, i32 w = -1, i32 h = -1) noexcept;
        void SetActive() noexcept;
        void SetVisible(bool visible = true) const noexcept;
        void Show() const noexcept;
        void Hide() const noexcept;

    protected:
        void SetCallbacks();
        std::string m_title;
        bool m_shouldClose = false;
        void *m_handle;
    };
}