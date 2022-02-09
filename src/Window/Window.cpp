#include "TML/Window/Window.h"
#include "TML/Window/Event.h"
#include "Headers/GLHeader.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "stb/stb_image_write.h"
#include "stb/stb_image.h"
#include "_Assert.h"

#include "Logo.h" /// Logo data
#include <cstring>

void DragAndDropCallback(GLFWwindow* window, int count, const char* files[]);
void MouseMoveCallback(GLFWwindow* window, double x, double y);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void CharCallback(GLFWwindow* window, unsigned int code);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void WindowResizeCallback(GLFWwindow* window, int x, int y);

namespace tml
{
    Window::Window(i32 w, i32 h, const std::string& title, ui32 settings)
    : m_handle(nullptr), m_title(title)
    {
        const auto result = glfwInit();
        TML_ASSERT(result, "Failed to initialize window.");
        #ifdef _GLFW_WAYLAND // We don't support wayland at the moment.
            Logger::ErrorMessage("Wayland not supported at the moment!");
            exit(66);
        #endif
        #if defined(TML_USE_GLES) && !defined(PLATFORM_WINDOWS)
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        #else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #endif

        m_useVSync = (settings & Settings::VSync) != 0;
        glfwWindowHint(GLFW_DECORATED,      (settings & Settings::NoTopBar)     == 0);
        glfwWindowHint(GLFW_RESIZABLE,      (settings & Settings::Resizeable)   != 0);
        glfwWindowHint(GLFW_MAXIMIZED,      (settings & Settings::Maximized)    != 0);
        glfwWindowHint(GLFW_DOUBLEBUFFER,   (settings & Settings::VSync)        != 0);
        glfwWindowHint(GLFW_SAMPLES, static_cast<int>(((settings & Settings::Antialias) >> 4) * 4));
        glfwSetErrorCallback([](int, const char* m){ Logger::ErrorMessage("GLFW ERROR: %s", m);});

        m_handle = glfwCreateWindow(w, h, title.c_str(),(settings & Settings::Fullscreen) ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        TML_ASSERT(m_handle != nullptr, "Failed to create a window handle.");
        auto handle = static_cast<GLFWwindow *>(m_handle);
        glfwMakeContextCurrent(handle);
        glfwShowWindow(handle);
        SetCallbacks();
        if((settings & Settings::LimitAspect) != 0)
            SetAspectRatio(w,h);
        GLFWimage img;
        int channels = 4;

        img.pixels = stbi_load_from_memory(
            LOGO_DATA.data(),
            static_cast<int>(LOGO_DATA.size()),
            &img.width,
            &img.height,
            &channels,
            4);
        glfwSetWindowIcon(handle,1, &img);
        delete[] img.pixels;
    }

    Window::~Window()
    {
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::Display()
    {
        if(m_useVSync)
            glfwSwapBuffers(static_cast<GLFWwindow*>(m_handle));
        else
            glad_glFlush();
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

    Event Window::PollEvents() noexcept
    {
        return EventSystem::GetInstance().PollEvents();
    }

    Event Window::WaitEvents() noexcept
    {
        return EventSystem::GetInstance().WaitEvents();
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

    void Window::Screenshot(const std::string& filename) const noexcept
    {
        const i32 w = GetWidth(), h = GetHeight();
        ui8* pixels = new ui8[3 * w * h];
        glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        stbi_flip_vertically_on_write(1);
        stbi_write_png(filename.c_str(), w, h, 3, pixels, 0);
        delete[] pixels;
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
        glfwSetWindowCloseCallback(handle, [](GLFWwindow* handle){
            Event event;
            event.type = Event::Closed;
            event.sender = handle;
            EventSystem::GetInstance().PushEvent(event);
        });
    }
}


/// GLFW callbacks

using tml::Event;

void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Event event{};
    event.type = Event::MouseWheelScrolled;
    event.mouseWheelScroll.x = 0;
    event.mouseWheelScroll.y = 0;
    event.mouseWheelScroll.delta = yoffset;
    event.sender = window;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void CharCallback(GLFWwindow* window, unsigned int code)
{
    Event event;
    event.type = Event::TextEntered;
    event.text.unicode = code;
    event.sender = window;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Event event;
    event.type = (action == GLFW_PRESS || action == GLFW_REPEAT) ? Event::KeyPressed : Event::KeyReleased;
    event.key.code = key;
    event.key.control = (mods == GLFW_MOD_CONTROL);
    event.key.shift = (mods == GLFW_MOD_SHIFT);
    event.sender = window;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    Event event;
    event.type = (action == GLFW_PRESS) ? Event::EventType::MouseButtonPressed : Event::EventType::MouseButtonReleased;
    event.mouseButton.button = button;
    event.mouseButton.x = x;
    event.mouseButton.y = y;
    event.sender = window;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void DragAndDropCallback(GLFWwindow* window, int count, const char** files)
{
    Event event;
    event.type = Event::FileDragAndDropped;
    event.dragAndDrop.count = count;
    event.dragAndDrop.paths = new char*[count];
    for(int i = 0; i < count; i++)
    {
        const auto str = std::string(files[i]);
        const auto len = str.size();

        event.dragAndDrop.paths[i] = new char[len]; /// Using new here
        std::memcpy(event.dragAndDrop.paths[i], str.data(), len);
    }
    event.sender = window;
    tml::EventSystem::GetInstance().PushEvent(event);
}

void WindowResizeCallback(GLFWwindow* window, int x, int y)
{
    tml::Event event;
    event.type = tml::Event::WindowResized;
    event.size.x = x;
    event.size.y = y;
    event.sender = window;
    tml::EventSystem::GetInstance().PushEvent(event);
}