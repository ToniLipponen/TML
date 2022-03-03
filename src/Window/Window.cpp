#include <TML/Window/Window.h>
#include <TML/System/Image.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "stb/stb_image_write.h"
#include "stb/stb_image.h"
#include "_Assert.h"

#include "Logo.h" /// Logo data
#include <cstring>

void DragAndDropCallback(GLFWwindow* window, int count, const char* files[]);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void CharCallback(GLFWwindow* window, unsigned int code);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void WindowResizeCallback(GLFWwindow* window, int x, int y);
void WindowFocusCallback(GLFWwindow* window, int focus);
void WindowCloseCallback(GLFWwindow* window);

namespace tml
{
    Window::Window(i32 w, i32 h, const std::string& title, ui32 settings)
    : m_handle(nullptr), m_title(title)
    {
        const auto result = glfwInit();
        TML_ASSERT(result, "Failed to initialize window.");

        #if defined(TML_USE_GLES)
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        #else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #endif

        glfwWindowHint(GLFW_DECORATED,      (settings & Settings::NoTopBar)     == 0);
        glfwWindowHint(GLFW_RESIZABLE,      (settings & Settings::Resizeable)   != 0);
        glfwWindowHint(GLFW_MAXIMIZED,      (settings & Settings::Maximized)    != 0);
        glfwWindowHint(GLFW_DOUBLEBUFFER,   (settings & Settings::VSync)        != 0);
        glfwWindowHint(GLFW_VISIBLE,        (settings & Settings::Hidden)       == 0);
        glfwWindowHint(GLFW_SAMPLES, static_cast<int>(((settings & Settings::Antialias) >> 4) * 8));
        glfwSetErrorCallback([](int, const char* m){ Logger::ErrorMessage("GLFW ERROR: %s", m);});

        m_handle = glfwCreateWindow(w, h, title.c_str(),(settings & Settings::Fullscreen) ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        TML_ASSERT(m_handle != nullptr, "Failed to create a window handle.");
        auto handle = static_cast<GLFWwindow *>(m_handle);
        glfwMakeContextCurrent(handle);
        SetCallbacks();
        if((settings & Settings::LimitAspect) != 0)
            SetAspectRatio(w,h);

        Image image(LOGO_DATA.data(), static_cast<int>(LOGO_DATA.size()));
        GLFWimage img;

        img.width = image.GetWidth();
        img.height = image.GetHeight();
        img.pixels = image.GetData();
        glfwSetWindowIcon(handle, 1, &img);
        SetSizeLimits(100,100,9000,9000);
    }

    Window::~Window()
    {
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::Display()
    {
        glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::Close() noexcept
    {
        m_shouldClose = true;
    }

    bool Window::ShouldClose() const noexcept
    {
        return m_shouldClose;
    }

    void *Window::GetHandle() noexcept
    {
        return m_handle;
    }

    i32 Window::GetWidth() const noexcept
    {
        i32 w, h;
        glfwGetWindowSize(static_cast<GLFWwindow*>(m_handle), &w, &h);
        return w;
    }

    i32 Window::GetHeight() const noexcept
    {
        i32 w, h;
        glfwGetWindowSize(static_cast<GLFWwindow*>(m_handle), &w, &h);
        return h;
    }

    i32 Window::GetX() const noexcept
    {
        i32 x, y;
        glfwGetWindowPos(static_cast<GLFWwindow*>(m_handle), &x, &y);
        return x;
    }

    i32 Window::GetY() const noexcept
    {
        i32 x, y;
        glfwGetWindowPos(static_cast<GLFWwindow*>(m_handle), &x, &y);
        return y;
    }

    bool Window::PollEvents(Event& e)
    {
        return EventSystem::GetInstance().PollEvents(e);
    }

    bool Window::WaitEvents(Event& e)
    {
        return EventSystem::GetInstance().WaitEvents(e);
    }

    Vector2i Window::GetPosition() const noexcept
    {
        i32 x, y;
        glfwGetWindowPos(static_cast<GLFWwindow*>(m_handle), &x, &y);
        return {x,y};
    }

    Vector2i Window::GetSize() const noexcept
    {
        i32 x, y;
        glfwGetWindowSize(static_cast<GLFWwindow*>(m_handle), &x, &y);
        return {x,y};
    }

    void Window::SetSize(ui32 w, ui32 h) noexcept
    {
        glfwRestoreWindow(static_cast<GLFWwindow*>(m_handle));
        glfwSetWindowSize(static_cast<GLFWwindow*>(m_handle), static_cast<int>(w), static_cast<int>(h));
    }

    void Window::SetTitle(const std::string& title) noexcept
    {
        glfwSetWindowTitle(static_cast<GLFWwindow *>(m_handle), title.c_str());
    }

    void Window::SetAspectRatio(i32 x, i32 y) noexcept
    {
        glfwSetWindowAspectRatio(static_cast<GLFWwindow*>(m_handle), x, y);
    }

    void Window::SetAspectRatio(const Vector2i &ratio) noexcept
    {
        glfwSetWindowAspectRatio(static_cast<GLFWwindow*>(m_handle), ratio.x, ratio.y);
    }

    void Window::SetSizeLimits(i32 minWidth, i32 minHeight, i32 maxWidth, i32 maxHeight) noexcept
    {
        glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(m_handle), minWidth, minHeight, maxWidth, maxHeight);
    }

    void Window::SetSizeLimits(const Vector2i &min, const Vector2i &max) noexcept
    {
        glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(m_handle), min.x, min.y, max.x, max.y);
    }

    void Window::Maximize() noexcept
    {
        glfwMaximizeWindow(static_cast<GLFWwindow *>(m_handle));
    }

    void Window::SetFullscreen(bool full, i32 user_w, i32 user_h) noexcept
    {
        int w = 0, h = 0, x = 0, y = 0;
        auto monitor = glfwGetPrimaryMonitor();
        // Destroy old handle
        glfwDestroyWindow(static_cast<GLFWwindow *>(m_handle));
        m_handle = nullptr;
        glfwGetMonitorWorkarea(monitor, &x, &y, &w, &h);
        m_handle = glfwCreateWindow(
                (user_w == -1) ? w : user_w,
                (user_h == -1) ? h : user_h,
                m_title.c_str(),
                monitor,
                nullptr);
        GLFWwindow* handle = static_cast<GLFWwindow*>(m_handle);
        TML_ASSERT(m_handle != nullptr, "Failed to create a window handle.");
        glfwMakeContextCurrent(handle);
        glfwShowWindow(handle);
        SetCallbacks();
    }

    void Window::SetActive() noexcept
    {
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

    void Window::SetCallbacks()
    {
        auto* handle = static_cast<GLFWwindow*>(m_handle);
        glfwSetWindowSizeCallback(handle, WindowResizeCallback);
        glfwSetDropCallback(handle, DragAndDropCallback);
        glfwSetCharCallback(handle, CharCallback);
        glfwSetKeyCallback(handle, KeyCallback);
        glfwSetMouseButtonCallback(handle, MouseButtonCallback);
        glfwSetScrollCallback(handle, MouseScrollCallback);
        glfwSetWindowFocusCallback(handle, WindowFocusCallback);
        glfwSetWindowCloseCallback(handle, WindowCloseCallback);
    }
}


/// GLFW callbacks

using tml::Event;

void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto mousePos = tml::Mouse::GetPosition();
    Event event{};
    event.type = Event::MouseWheelScrolled;
    event.mouseWheelScroll.x = mousePos.x;
    event.mouseWheelScroll.y = mousePos.y;
    event.mouseWheelScroll.delta = yoffset;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void CharCallback(GLFWwindow* window, unsigned int code)
{
    Event event{};
    event.type = Event::TextEntered;
    event.text.unicode = code;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Event event{};
    event.type = (action == GLFW_PRESS || action == GLFW_REPEAT) ? Event::KeyPressed : Event::KeyReleased;
    event.key.code = key;
    event.key.control = (mods == GLFW_MOD_CONTROL);
    event.key.shift = (mods == GLFW_MOD_SHIFT);
    tml::EventSystem::GetInstance().PushEvent(event);
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
    tml::EventSystem::GetInstance().PushEvent(event);
}

void DragAndDropCallback(GLFWwindow* window, int count, const char** files)
{
    Event event{};
    event.type = Event::FileDragAndDropped;
    event.dragAndDrop.count = count;
    event.dragAndDrop.paths = new char*[count];
    for(int i = 0; i < count; i++)
    {
        const auto str = tml::String(files[i]);
        const auto len = str.size();

        event.dragAndDrop.paths[i] = new char[len+1]; /// Using new here
        event.dragAndDrop.paths[i][len] = 0;
        std::memcpy(event.dragAndDrop.paths[i], str.data(), len);
    }
    tml::EventSystem::GetInstance().PushEvent(event);
}

void WindowResizeCallback(GLFWwindow* window, int x, int y)
{
    tml::Event event{};
    event.type = tml::Event::WindowResized;
    event.size.x = x;
    event.size.y = y;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void WindowFocusCallback(GLFWwindow* window, int focus)
{
    Event event{};
    event.type = focus ? Event::GainedFocus : Event::LostFocus;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void WindowCloseCallback(GLFWwindow* window)
{
    Event event{};
    event.type = Event::Closed;
    tml::EventSystem::GetInstance().PushEvent(event);
}