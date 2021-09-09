#pragma once
#include "Types.h"
#include "Input.h"
#include <string>

namespace tml {
    class Window {
    public:
        enum Settings {
            None = 0x0,
            Fullscreen = 0x1,
            Resizeable = 0x2,
            Maximized = 0x4,
            VSync = 0x8,
            Antialias = 0x10
        };
    public:
        Window(ui32 width, ui32 height, cstring title, ui32 settings);
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
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
        void SetTitle(cstring title);
        void Maximize();
        void SetFullscreen(bool fullscreen, i32 w = -1, i32 h = -1);
        void Screenshot(const cstring filename);
    private:
        std::string m_title;
        void *m_handle;
    };
};