#pragma once
#include "TML/Utilities/Types.h"
#include "TML/IO/Input.h"
#include "TML/IO/Event.h"

namespace tml
{
    class Window
    {
    public:
        enum Settings
        {
            None        = 0,
            Fullscreen  = (1 << 0),
            Resizeable  = (1 << 1),
            Maximized   = (1 << 2),
            VSync       = (1 << 3),
            Antialias   = (1 << 4),
            NoTopBar    = (1 << 5)
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

        Event PollEvents() const noexcept;
        Event WaitEvents() const noexcept;

        Vector2i GetPosition() const noexcept;
        Vector2i GetSize() const noexcept;

        void SetSize(ui32 width, ui32 height) noexcept;
        void SetTitle(const std::string& title) noexcept;
        void Maximize() noexcept;
        void SetFullscreen(bool fullscreen, i32 w = -1, i32 h = -1) noexcept;
        void SetActive() noexcept;
        void Screenshot(const std::string& filename) const noexcept;
    private:
        void SetCallbacks();
        std::string m_title;
        bool m_useVSync = false;
        bool m_shouldClose = false;
        void *m_handle;
    };
}