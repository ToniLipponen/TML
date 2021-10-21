#pragma once
#include "Types.h"
#include "TML/IO/Input.h"

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
        void Close() const noexcept;
        bool ShouldClose() const noexcept;
        
        void *GetHandle() noexcept;
        i32 GetWidth() const noexcept;
        i32 GetHeight() const noexcept;
        i32 GetX() const noexcept;
        i32 GetY() const noexcept;
        double GetTime() const noexcept;

        void SetSize(ui32 width, ui32 height) noexcept;
        void SetFpsLimit(ui32 fps);
        void SetTitle(cstring title);
        void Maximize();
        void SetFullscreen(bool fullscreen, i32 w = -1, i32 h = -1);
        void SetActive();
        void Screenshot(const std::string& filename);
    private:
        std::string m_title;
        void *m_handle;
    };
}