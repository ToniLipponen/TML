#pragma once
#include "Types.h"
#include "Input.h"

class Window
{
public:
    enum Setting
    {
        None        = 0x0,
        Fullscreen  = 0x1,
        Resizeable  = 0x2,
        Maximized   = 0x4,
        VSync       = 0x8,
        Antialias   = 0x10
    };
public:
    Window(ui32 width, ui32 height, cstring title, ui32 settings);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    virtual ~Window();
    virtual void Display();
    void Close() const noexcept;
    bool ShouldClose() const noexcept;

    // Getters
    void* GetHandle() noexcept;
    i32 GetWidth() const noexcept;
    i32 GetHeight() const noexcept;
    i32 GetX() const noexcept;
    i32 GetY() const noexcept;
    double GetTime() const noexcept;
    
    // Setters
    void SetSize(ui32 width, ui32 height) noexcept;
    void SetTitle(cstring title);
    void Maximize();
    // void SetFullscreen(bool fullscreen);
private:
    void* m_handle;
};