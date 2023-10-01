#include <TML/Window/Window.h>
#include <TML/System/Math.h>
#include <TML/TMLAssert.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Logo.h" /// Logo data
#include "GLContext/GLContext.h"

static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
static void CharCallback(GLFWwindow* window, unsigned int code);
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void WindowResizeCallback(GLFWwindow* window, int x, int y);
static void WindowFocusCallback(GLFWwindow* window, int focus);
static void WindowCloseCallback(GLFWwindow* window);
static void CursorEnterCallback(GLFWwindow* window, int entered);
static void WindowMaximizeCallback(GLFWwindow* window, int maximized);
static void WindowMinimizeCallback(GLFWwindow* window, int minimized);
static void CursorPosCallback(GLFWwindow* window, double x, double y);
static void WindowMoveCallback(GLFWwindow* window, int x, int y);
static void DropCallback(GLFWwindow* window, int pathCount, const char** paths);

namespace tml
{
    Window::Window() noexcept
    : m_handle(nullptr)
    {
        
    }

    Window::Window(int32_t w, int32_t h, const String& title, int32_t flags) noexcept
    : m_handle(nullptr)
    {
        TML_ASSERT(Create(w, h, title, flags), "Failed to create a window");
    }

    Window::Window(const WindowSettings& settings) noexcept
    : m_handle(nullptr)
    {
        Create(settings);
    }

    Window::~Window() noexcept
    {
        Close();
    }

    void Window::Display() noexcept
    {
        /// Swap buffers if this window is still open and there is an opengl context associated with this window.
        if(IsOpen() && m_hasGLContext)
        {
            glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
        }
    }

    bool Window::Create(int32_t w, int32_t h, const String& title, int32_t flags) noexcept
    {
        WindowSettings settingsStruct{};
        settingsStruct.size = Vector2i(w, h);
        settingsStruct.title = title;
        settingsStruct.icon = Image(LOGO_DATA, sizeof(LOGO_DATA));
        settingsStruct.flags = flags;
        settingsStruct.maximumSize = Vector2i(8192, 8192);
        settingsStruct.minimumSize = Vector2i(100, 100);

        if(flags & WindowSettings::Antialias)
        {
            settingsStruct.antialiasSamples = 4;
        }

        return Create(settingsStruct);
    }

    bool Window::Create(const WindowSettings &settings) noexcept
    {
        m_settings = settings;
        GLFWwindow* glContextHandle = nullptr;
        int flags = settings.flags;
        String title = settings.title;

        if(flags & WindowSettings::NoClient)
        {
            TML_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }
        else
        {
            glContextHandle = static_cast<GLFWwindow*>(GLContext::GetInstance().GetContextHandle());
            m_hasGLContext = (glContextHandle != nullptr); //!< If glContextHandle is nullptr, a context was not created.
        }

        glfwWindowHint(GLFW_DECORATED,                 (flags & WindowSettings::NoTopBar)     == 0);
        glfwWindowHint(GLFW_RESIZABLE,                 (flags & WindowSettings::Resizeable)   != 0);
        glfwWindowHint(GLFW_MAXIMIZED,                 (flags & WindowSettings::Maximized)    != 0);
        glfwWindowHint(GLFW_VISIBLE,                   (flags & WindowSettings::Hidden)       == 0);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,   (flags & WindowSettings::Transparent)  != 0);
        glfwWindowHint(GLFW_FLOATING,                  (flags & WindowSettings::AlwaysOnTop)  != 0);
        glfwWindowHint(GLFW_DOUBLEBUFFER,              (flags & WindowSettings::VSync)        != 0);
        glfwWindowHint(GLFW_AUTO_ICONIFY,              (flags & WindowSettings::Minimized)     != 0);

        if(flags & WindowSettings::Antialias)
        {
            glfwWindowHint(GLFW_SAMPLES, Math::Clamp<int>(settings.antialiasSamples, 0, 16));
        }

        GLFWmonitor* monitorHandle = nullptr;

        if(flags & WindowSettings::Fullscreen)
        {
            if(settings.monitor)
            {
                monitorHandle = static_cast<GLFWmonitor*>(settings.monitor->GetHandle());
            }
            else
            {
                monitorHandle = static_cast<GLFWmonitor*>(GetPrimaryMonitor().GetHandle());
            }
        }

        if(flags & WindowSettings::UseMonitorResolution)
        {
            Vector2i workAreaSize;
            Vector2i workAreaPos;
            Vector2i pos;

            if(settings.monitor) //!< Checking if user provided a monitor in settings.
            {
                workAreaSize = settings.monitor->GetWorkAreaSize();
                workAreaPos = settings.monitor->GetWorkAreaPosition();
                pos = settings.monitor->GetPosition();
            }
            else //!< Use the primary monitor.
            {
                auto primaryMonitor = GetPrimaryMonitor();
                workAreaSize = primaryMonitor.GetWorkAreaSize();
                workAreaPos = primaryMonitor.GetWorkAreaPosition();
                pos = primaryMonitor.GetPosition();
            }

            m_size = Vector2i(workAreaSize.x + workAreaPos.x - pos.x, workAreaSize.y + workAreaPos.y - pos.y);
            m_handle = glfwCreateWindow(m_size.x, m_size.y, title.c_str(), monitorHandle, glContextHandle);
        }
        else
        {
            m_handle = glfwCreateWindow(settings.size.x, settings.size.y, title.c_str(), monitorHandle, glContextHandle);
            m_size = settings.size;
        }

        if(m_handle == nullptr)
        {
            return false;
        }

        SetActive();
        glfwSwapInterval(static_cast<int>((flags & WindowSettings::VSync) != 0));

        /** Set icon **/
        {
            Image icon;

            if(settings.icon)
            {
                icon = settings.icon.value();
            }
            else
            {
                icon.LoadFromData(LOGO_DATA, sizeof(LOGO_DATA));
            }
        }

        /** Set window limits **/
        {
            if(flags & WindowSettings::LimitAspect)
            {
                SetAspectRatio(settings.size.x, settings.size.y);
            }

            SetSizeLimits(settings.minimumSize.value_or(Vector2i(100, 100)), settings.maximumSize.value_or(Vector2i(8192, 8192)));
        }

        if(settings.position)
        {
            SetPosition(settings.position->x, settings.position->y);
        }

        SetCallbacks();
        glfwGetWindowPos(static_cast<GLFWwindow*>(m_handle), &m_pos.x, &m_pos.y);
        glfwGetWindowSize(static_cast<GLFWwindow*>(m_handle), &m_size.x, &m_size.y);
        return m_handle != nullptr;
    }

    void Window::Close() noexcept
    {
        SetActive(false);
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_handle));
        m_handle = nullptr;
    }

    bool Window::IsOpen(bool waitForEvent) const noexcept
    {
        if(m_handle)
        {
            SetActive(true);
            
            if(waitForEvent)
            {
                glfwWaitEvents();
            }
            else
            {
                glfwPollEvents();
            }

            return !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
        }

        return false;
    }

	bool Window::IsMaximized() const noexcept
	{
		return glfwGetWindowAttrib(static_cast<GLFWwindow*>(m_handle), GLFW_MAXIMIZED) != 0;
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

    Vector2i Window::GetSize() const noexcept
    {
        return m_size;
    }

    Vector2i Window::GetPosition() const noexcept
    {
        return m_pos;
    }

    const WindowSettings& Window::GetSettings() const noexcept
    {
        return m_settings;
    }

	Monitor Window::GetMonitor() const noexcept
	{
		return glfwGetWindowMonitor(static_cast<GLFWwindow*>(m_handle));
	}

	Vector2f Window::GetContentScale() const noexcept
	{
		Vector2f scale{0};
		glfwGetWindowContentScale(static_cast<GLFWwindow*>(m_handle), &scale.x, &scale.y);

		return scale;
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
        img.pixels = (uint8_t*)image.GetData();

        glfwSetWindowIcon(static_cast<GLFWwindow*>(m_handle), 1, &img);
    }

    void Window::SetCursor(const Cursor& cursor) noexcept
    {
        glfwSetCursor(static_cast<GLFWwindow*>(m_handle), static_cast<GLFWcursor*>(cursor.GetHandle()));
    }

    void Window::ResetCursor() noexcept
    {
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

    void Window::SetSizeLimits(const Vector2i& min, const Vector2i& max) noexcept
    {
        const int minWidth = Math::Max(min.x, 0);
        const int minHeight = Math::Max(min.y, 0);
        const int maxWidth = Math::Max(max.x, minWidth);
        const int maxHeight = Math::Max(max.y, minHeight);

        glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(m_handle), minWidth, minHeight, maxWidth, maxHeight);
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

    void Window::SetFullscreen(bool full, const std::optional<Monitor>& monitor) noexcept
    {
        if(full)
        {
            Vector2i size;
            GLFWmonitor* monitorHandle;

            if(monitor)
            {
                size = monitor->GetSize();
                monitorHandle = static_cast<GLFWmonitor*>(monitor->GetHandle());
            }
            else if(m_settings.monitor) //<! Use the monitor provided with WindowSettings.
            {
                size = m_settings.monitor->GetSize();
                monitorHandle = static_cast<GLFWmonitor*>(m_settings.monitor->GetHandle());
            }
            else //<! Use the primary monitor
            {
                Monitor primaryMonitor = GetPrimaryMonitor();
                size = primaryMonitor.GetSize();
                monitorHandle = static_cast<GLFWmonitor*>(primaryMonitor.GetHandle());
            }

            glfwSetWindowMonitor(static_cast<GLFWwindow*>(m_handle),
                                 monitorHandle,
                                 0,
                                 0,
                                 size.x,
                                 size.y,
                                 GLFW_DONT_CARE);
        }
        else
        {
            glfwSetWindowMonitor(static_cast<GLFWwindow*>(m_handle),
                                 nullptr,
                                 m_pos.x,
                                 m_pos.y,
                                 m_size.x,
                                 m_size.y,
                                 GLFW_DONT_CARE);
        }
    }

    void Window::SetActive(bool active) const noexcept
    {
        if(!active)
        {
            glfwMakeContextCurrent(nullptr);
        }
        else if(m_handle != glfwGetCurrentContext() && m_hasGLContext)
        {
            glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_handle));
        }
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

    // void Window::HandleWindowEvents(const Event &e) noexcept
    // {
    //     switch(e.type)
    //     {
    //         case tml::Event::WindowResized:
    //             m_size = Vector2i{e.size.w, e.size.h};
    //             break;

    //         case tml::Event::WindowMoved:
    //             m_pos = Vector2i{e.pos.x, e.pos.y};
    //             break;

    //         case tml::Event::MouseButtonPressed:
    //         {
    //             m_mouseDown = true;
    //             m_buttonDown = static_cast<Mouse::Button>(e.mouseButton.button);
    //             m_mouseDownPos = {e.mouseButton.x, e.mouseButton.y};
    //         } break;

    //         case tml::Event::MouseButtonReleased:
    //         {
    //             m_mouseDown = false;
    //             m_dragging = false;

    //             if(!m_dragging)
    //             {
    //                 Event clickEvent{};
    //                 clickEvent.type = tml::Event::MouseButtonClicked;
    //                 clickEvent.mouseButton.button = static_cast<tml::Mouse::Button>(m_buttonDown);
    //                 clickEvent.mouseButton.x = e.mouseButton.x;
    //                 clickEvent.mouseButton.y = e.mouseButton.y;

    //                 tml::EventSystem::GetInstance().PushEvent(m_handle, clickEvent);
    //             }
    //         } break;

    //         case tml::Event::MouseMoved:
    //         {
    //             if(m_mouseDown)
    //             {
    //                 Event dragEvent{};
    //                 dragEvent.type = tml::Event::MouseDragged;
    //                 dragEvent.drag.button = static_cast<tml::Mouse::Button>(m_buttonDown);
    //                 dragEvent.drag.x = e.pos.x;
    //                 dragEvent.drag.y = e.pos.y;
    //                 dragEvent.drag.beginX = m_mouseDownPos.x;
    //                 dragEvent.drag.beginY = m_mouseDownPos.y;

    //                 if(!m_dragging && Math::Distance(m_mouseDownPos, Vector2i(e.mouseButton.x, e.mouseButton.y)) > m_minDragDistance)
    //                 {
    //                     m_dragging = true;
    //                 }

    //                 tml::EventSystem::GetInstance().PushEvent(m_handle, dragEvent);
    //             }
    //         } break;

    //         default:
    //             break;
    //     }
    // }

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
        glfwSetDropCallback(handle, DropCallback);
        glfwSetWindowUserPointer(handle, this);
    }
}

static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto mousePos = tml::Mouse::GetPosition();
    tml::MouseScrollEvent e{};
    e.x      = static_cast<int>(mousePos.x);
    e.y      = static_cast<int>(mousePos.y);
    e.deltaY = static_cast<float>(yoffset);
    e.deltaX = static_cast<float>(xoffset);

    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tmlWindow->OnMouseScrolled.Invoke(tmlWindow, e);
}

static void CharCallback(GLFWwindow* window, unsigned int code)
{
    tml::TextEvent e;
    e.unicode = code;

    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tmlWindow->OnTextEntered.Invoke(tmlWindow, e);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));

    tml::KeyEvent event{};
    event.value     = static_cast<tml::Keyboard::Key>(key);
    event.code      = scancode;
    event.control   = (mods & GLFW_MOD_CONTROL) != 0;
    event.shift     = (mods & GLFW_MOD_SHIFT)   != 0;
    event.alt       = (mods & GLFW_MOD_ALT)     != 0;
    event.system    = (mods & GLFW_MOD_SUPER)   != 0;

    if(action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        tmlWindow->OnKeyPressed.Invoke(tmlWindow, event);
    }
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::MouseButtonEvent e;
    e.button = static_cast<tml::Mouse::Button>(button);
    e.x = static_cast<int>(x);
    e.y = static_cast<int>(y);

    tmlWindow->OnMouseDown.Invoke(tmlWindow, e);
}

static void WindowResizeCallback(GLFWwindow* window, int x, int y)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::ResizeEvent e;
    e.w = x;
    e.h = y;
    tmlWindow->OnResized.Invoke(tmlWindow, e);
}

static void WindowFocusCallback(GLFWwindow* window, int focus)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::Event e;
    
    if(focus)
    {
        tmlWindow->OnGainedFocus.Invoke(tmlWindow, e);
    }
    else
    {
        tmlWindow->OnLostFocus.Invoke(tmlWindow, e);
    }
}

static void WindowCloseCallback(GLFWwindow* window)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::Event e;

    tmlWindow->OnClosed.Invoke(tmlWindow, e);
}

static void CursorEnterCallback(GLFWwindow* window, int entered)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::Event e;

    if(entered)
    {
        tmlWindow->OnMouseEnter.Invoke(tmlWindow, e);
    }
    else
    {
        tmlWindow->OnMouseExit.Invoke(tmlWindow, e);
    }
}

static void WindowMaximizeCallback(GLFWwindow* window, int maximized)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::Event e;

    if(maximized)
    {
        tmlWindow->OnMaximized.Invoke(tmlWindow, e);
    }
    else
    {
        tmlWindow->OnRestored.Invoke(tmlWindow, e);
    }
}

static void WindowMinimizeCallback(GLFWwindow* window, int minimized)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::Event e;
    
    if(minimized)
    {
        tmlWindow->OnMinimized.Invoke(tmlWindow, e);
    }
    else
    {
        tmlWindow->OnRestored.Invoke(tmlWindow, e);
    }
}

static void CursorPosCallback(GLFWwindow* window, double x, double y)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::MoveEvent e;
    e.x = static_cast<int>(x);
    e.y = static_cast<int>(y);
    tmlWindow->OnMouseMoved.Invoke(tmlWindow, e);
}

static void WindowMoveCallback(GLFWwindow* window, int x, int y)
{
    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tml::MoveEvent e;
    e.x = x;
    e.y = y;
    tmlWindow->OnMoved.Invoke(tmlWindow, e);
}

static void DropCallback(GLFWwindow* window, int pathCount, const char** paths)
{
    tml::FileDropEvent e;

    for(int i = 0; i < pathCount; i++)
    {
        e.files.push_back(paths[i]);
    }

    auto tmlWindow = static_cast<tml::Window*>(glfwGetWindowUserPointer(window));
    tmlWindow->OnFilesDropped.Invoke(tmlWindow, e);
}