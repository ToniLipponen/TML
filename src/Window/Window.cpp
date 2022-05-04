#include <TML/Window/Window.h>
#include <TML/System/Math.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../Headers/_Assert.h"
#include "../Headers/Logo.h" /// Logo data
#include "GLContext/GLContext.h"

void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void CharCallback(GLFWwindow* window, unsigned int code);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void WindowResizeCallback(GLFWwindow* window, int x, int y);
void WindowFocusCallback(GLFWwindow* window, int focus);
void WindowCloseCallback(GLFWwindow* window);
void CursorEnterCallback(GLFWwindow* window, int entered);
void WindowMaximizeCallback(GLFWwindow* window, int maximized);
void WindowMinimizeCallback(GLFWwindow* window, int minimized);
void CursorPosCallback(GLFWwindow* window, double x, double y);
void WindowMoveCallback(GLFWwindow* window, int x, int y);
void GamepadCallback(int jid, int event);

namespace tml
{
    Window::Window()
    : m_handle(nullptr)
    {

    }

    Window::Window(int32_t w, int32_t h, const String& title, uint32_t settings)
    : m_handle(nullptr)
    {
        if(!Create(w, h, title, settings))
            Logger::ErrorMessage("Failed to create a window");
    }

    Window::~Window() noexcept
    {
        Close();
    }

    void Window::Display() noexcept
    {
        /// Swap buffers if this window is still open and there is an opengl context associated with this window.
        if(IsOpen() && m_hasGLContext)
            glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
    }

    bool Window::Create(int32_t w, int32_t h, const String& title, uint32_t settings) noexcept
    {
        GLFWwindow* glContextHandle = nullptr;
        if(settings & NoClient)
        {
            TML_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }
        else
        {
            glContextHandle = static_cast<GLFWwindow*>(GLContext::GetInstance().GetContextHandle());
            m_hasGLContext = (glContextHandle != nullptr); //!< If glContext handle is nullptr, a context was not created.
        }

        glfwWindowHint(GLFW_DECORATED,                 (settings & Settings::NoTopBar)     == 0);
        glfwWindowHint(GLFW_RESIZABLE,                 (settings & Settings::Resizeable)   != 0);
        glfwWindowHint(GLFW_MAXIMIZED,                 (settings & Settings::Maximized)    != 0);
        glfwWindowHint(GLFW_VISIBLE,                   (settings & Settings::Hidden)       == 0);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,   (settings & Settings::Transparent)  != 0);
        glfwWindowHint(GLFW_FLOATING,                  (settings & Settings::AlwaysOnTop)  != 0);
        glfwWindowHint(GLFW_SAMPLES, static_cast<int>(((settings & Settings::Antialias) >> 4) * 8));
        glfwWindowHint(GLFW_DOUBLEBUFFER,              (settings & Settings::VSync)        != 0);

        auto* primaryMonitor = glfwGetPrimaryMonitor();
        auto* monitor = (settings & Settings::Fullscreen) ? primaryMonitor : nullptr;

        if(settings & Settings::UseMonitorResolution)
            glfwGetMonitorWorkarea(primaryMonitor, nullptr, nullptr, &w, &h);

        /// Create the actual window
        m_handle = glfwCreateWindow(w, h, title.c_str(), monitor, glContextHandle);
        if(m_handle == nullptr)
            return false;

        m_size = {w, h};
        m_pos  = {0,0};

        auto handle = static_cast<GLFWwindow*>(m_handle);
        glfwMakeContextCurrent(handle);

        if((settings & Settings::VSync) != 0)
            glfwSwapInterval(1);
        glfwSwapInterval(1);

        /** Set window icon to TML-logo **/
        Image image(LOGO_DATA.data(), static_cast<int>(LOGO_DATA.size()));
        SetIcon(image);

        if((settings & Settings::LimitAspect) != 0)
            SetAspectRatio(w,h);
        if(settings & LimitSize)
            SetSizeLimits(w, h, 8192, 8192);
        else
            SetSizeLimits(100, 100, 8192, 8192);

        SetCallbacks();
        return true;
    }

    void Window::Close() noexcept
    {
        tml::EventSystem::GetInstance().Remove(m_handle);
        SetActive(false);
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_handle));
        m_handle = nullptr;

        glfwMakeContextCurrent((GLFWwindow*)GLContext::GetInstance().GetContextHandle());
    }

    bool Window::IsOpen() const noexcept
    {
        if(m_handle)
            return !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
        
        return false;
    }

    const void* Window::GetHandle() const noexcept
    {
        return m_handle;
    }

    int32_t Window::GetWidth() const noexcept
    {
        return m_size.x;
    }

    int32_t Window::GetHeight() const noexcept
    {
        return m_size.y;
    }

    int32_t Window::GetX() const noexcept
    {
        return m_pos.x;
    }

    int32_t Window::GetY() const noexcept
    {
        return m_pos.y;
    }

    bool Window::PollEvents(Event& e) noexcept
    {
        const auto returnValue = EventSystem::GetInstance().PollEvents(m_handle, e);
        HandleWindowEvents(e);
        return returnValue;
    }

    bool Window::WaitEvents(Event& e) noexcept
    {
        const auto returnValue = EventSystem::GetInstance().WaitEvents(m_handle, e);
        HandleWindowEvents(e);
        return returnValue;
    }

    Vector2i Window::GetPosition() const noexcept
    {
        return m_pos;
    }

    Vector2i Window::GetSize() const noexcept
    {
        return m_size;
    }

    void Window::SetPosition(int32_t x, int32_t y) noexcept
    {
        glfwSetWindowPos(static_cast<GLFWwindow*>(m_handle), x, y);
    }

    void Window::SetSize(uint32_t w, uint32_t h) noexcept
    {
        glfwRestoreWindow(static_cast<GLFWwindow*>(m_handle));
        glfwSetWindowSize(static_cast<GLFWwindow*>(m_handle), static_cast<int>(w), static_cast<int>(h));
    }

    void Window::SetTitle(const String& title) noexcept
    {
        glfwSetWindowTitle(static_cast<GLFWwindow *>(m_handle), title.c_str());
    }

    void Window::SetIcon(const Image& image) const noexcept
    {
        GLFWimage img;
        img.width = image.GetWidth();
        img.height = image.GetHeight();
        img.pixels = image.GetData();

        glfwSetWindowIcon(static_cast<GLFWwindow*>(m_handle), 1, &img);
    }

    void Window::SetCursor(const Image& image) noexcept
    {
        if(m_cursor)
        {
            glfwDestroyCursor(static_cast<GLFWcursor*>(m_cursor));
            m_cursor = nullptr;
        }

        GLFWimage img;
        img.width = image.GetWidth();
        img.height = image.GetHeight();
        img.pixels = image.GetData();

        m_cursor = glfwCreateCursor(&img, 0, 0);
        glfwSetCursor(static_cast<GLFWwindow*>(m_handle), static_cast<GLFWcursor*>(m_cursor));
    }

    void Window::ResetCursor() noexcept
    {
        glfwDestroyCursor(static_cast<GLFWcursor*>(m_cursor));
        glfwSetCursor(static_cast<GLFWwindow*>(m_handle), nullptr);
    }

    void Window::HideCursor() noexcept
    {
        glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void Window::ShowCursor() noexcept
    {
        glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Window::SetAspectRatio(int32_t x, int32_t y) noexcept
    {
        glfwSetWindowAspectRatio(static_cast<GLFWwindow*>(m_handle), x, y);
    }

    void Window::SetAspectRatio(const Vector2i &ratio) noexcept
    {
        glfwSetWindowAspectRatio(static_cast<GLFWwindow*>(m_handle), ratio.x, ratio.y);
    }

    void Window::SetSizeLimits(int32_t minWidth, int32_t minHeight, int32_t maxWidth, int32_t maxHeight) noexcept
    {
        glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(m_handle), minWidth, minHeight, maxWidth, maxHeight);
    }

    void Window::SetSizeLimits(const Vector2i &min, const Vector2i &max) noexcept
    {
        glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(m_handle), min.x, min.y, max.x, max.y);
    }

    void Window::Minimize() const noexcept
    {
        glfwIconifyWindow(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::Maximize() const noexcept
    {
        glfwMaximizeWindow(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::Restore() const noexcept
    {
        glfwRestoreWindow(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::SetFullscreen(bool full, int32_t user_w, int32_t user_h) noexcept
    {
        int w = 0, h = 0, x = 0, y = 0;
        auto monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorWorkarea(monitor, &x, &y, &w, &h);

        w = (user_w > 0) ? Math::Min<int32_t>(user_w, w) : w;
        h = (user_h > 0) ? Math::Min<int32_t>(user_h, h) : h;

        glfwSetWindowMonitor(static_cast<GLFWwindow*>(m_handle), full ? monitor : nullptr, x, y, w, h, GLFW_DONT_CARE);
    }

    void Window::SetActive(bool active) const noexcept
    {
        if(!active)
            glfwMakeContextCurrent(nullptr);
        else if(m_handle != glfwGetCurrentContext())
            glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::SetVisible(bool visible) const noexcept
    {
        visible ? Show() : Hide();
    }

    void Window::Show() const noexcept
    {
        glfwShowWindow(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::Hide() const noexcept
    {
        glfwHideWindow(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::HandleWindowEvents(const Event &e) noexcept
    {
        switch(e.type)
        {
            case Event::WindowResized:
                m_size = Vector2i{e.size.w, e.size.h};
                break;

            case Event::WindowMoved:
                m_pos = Vector2i{e.pos.x, e.pos.y};
                break;

            default:
                break;
        }
    }

    void Window::SetCallbacks() noexcept
    {
        auto* handle = static_cast<GLFWwindow*>(m_handle);
        glfwSetWindowSizeCallback(handle, WindowResizeCallback);
        glfwSetCharCallback(handle, CharCallback);
        glfwSetKeyCallback(handle, KeyCallback);
        glfwSetMouseButtonCallback(handle, MouseButtonCallback);
        glfwSetScrollCallback(handle, MouseScrollCallback);
        glfwSetWindowFocusCallback(handle, WindowFocusCallback);
        glfwSetWindowCloseCallback(handle, WindowCloseCallback);
        glfwSetCursorEnterCallback(handle, CursorEnterCallback);
        glfwSetWindowMaximizeCallback(handle, WindowMaximizeCallback);
        glfwSetWindowIconifyCallback(handle, WindowMinimizeCallback);
        glfwSetCursorPosCallback(handle, CursorPosCallback);
        glfwSetWindowPosCallback(handle, WindowMoveCallback);
        glfwSetJoystickCallback(GamepadCallback);
        tml::EventSystem::GetInstance().Register(handle);
    }
}

/// GLFW callbacks
using tml::Event;

void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto mousePos = tml::Mouse::GetPosition();
    Event event{};
    event.type = Event::MouseWheelScrolled;

    event.mouseWheelScroll.x      = static_cast<int>(mousePos.x);
    event.mouseWheelScroll.y      = static_cast<int>(mousePos.y);
    event.mouseWheelScroll.deltaY = static_cast<float>(yoffset);
    event.mouseWheelScroll.deltaX = static_cast<float>(xoffset);

    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void CharCallback(GLFWwindow* window, unsigned int code)
{
    Event event{};
    event.type = Event::TextEntered;
    event.text.unicode = code;
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Event event{};
    event.type = (action == GLFW_PRESS || action == GLFW_REPEAT) ? Event::KeyPressed : Event::KeyReleased;
    event.key.value = key;
    event.key.code = scancode;

    event.key.control   = (mods & GLFW_MOD_CONTROL) != 0;
    event.key.shift     = (mods & GLFW_MOD_SHIFT)   != 0;
    event.key.alt       = (mods & GLFW_MOD_ALT)     != 0;
    event.key.system    = (mods & GLFW_MOD_SUPER)   != 0;

    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    Event event{};
    event.type = (action == GLFW_PRESS) ? Event::EventType::MouseButtonPressed : Event::EventType::MouseButtonReleased;
    event.mouseButton.button = button;
    event.mouseButton.x = x;
    event.mouseButton.y = y;
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void WindowResizeCallback(GLFWwindow* window, int x, int y)
{
    tml::Event event{};
    event.type = tml::Event::WindowResized;
    event.size.w = x;
    event.size.h = y;
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void WindowFocusCallback(GLFWwindow* window, int focus)
{
    Event event{};
    event.type = focus ? Event::GainedFocus : Event::LostFocus;
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void WindowCloseCallback(GLFWwindow* window)
{
    Event event{};
    event.type = Event::Closed;
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void CursorEnterCallback(GLFWwindow* window, int entered)
{
    Event event{};
    event.type = entered ? Event::MouseEntered : Event::MouseLeft;
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void WindowMaximizeCallback(GLFWwindow* window, int maximized)
{
    Event event{};
    event.type = maximized ? Event::WindowMaximized : Event::WindowRestored;
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void WindowMinimizeCallback(GLFWwindow* window, int minimized)
{
    Event event{};
    event.type = minimized ? Event::WindowMinimized : Event::WindowRestored;
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void CursorPosCallback(GLFWwindow* window, double x, double y)
{
    Event event{};
    event.type = Event::MouseMoved;
    event.pos = {static_cast<int>(x), static_cast<int>(y)};
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void WindowMoveCallback(GLFWwindow* window, int x, int y)
{
    Event event{};
    event.type = Event::WindowMoved;
    event.pos = {x, y};
    tml::EventSystem::GetInstance().PushEvent(window, event);
}

void GamepadCallback(int jid, int glfwEvent)
{
    Event event{};
    event.type = glfwEvent == GLFW_CONNECTED ? Event::GamepadConnected : Event::GamepadDisconnected;
    event.gamepad.id = jid;
    tml::EventSystem::GetInstance().PushGlobalEvent(event);
}