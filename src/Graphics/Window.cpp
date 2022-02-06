#include <TML/Graphics/Window.h>
#include <TML/Utilities/Condition.h>
#include <TML/Utilities/Platform.h>
#include <TML/IO/Event.h>

#define GLAD_GL_IMPLEMENTATION
#define GLAD_GLES2_IMPLEMENTATION
#define GLAD_EGL_IMPLEMENTATION
#define GLAD_WGL_IMPLEMENTATION
#define GLAD_GLX_IMPLEMENTATION
#include <GLHeader.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb/stb_image_write.h>
#include <stb/stb_image.h>
#include "_Assert.h"

#if PLATFORM_UNIX
    #include <incbin/incbin.h>
    INCBIN(TML_ICON, "../res/Logo.png");
#else
    #include <Logo.h>
#endif

extern "C" void DragAndDropCallback(GLFWwindow* window, int count, const char* files[]);
extern "C" void MouseMoveCallback(GLFWwindow* window, double x, double y);
extern "C" void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
extern "C" void CharCallback(GLFWwindow* window, unsigned int code);
extern "C" void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern "C" void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void WindowResizeCallback(GLFWwindow* f, int x, int y)
{
    tml::Event event;
    event.type = tml::Event::WindowResized;
    event.size.x = x;
    event.size.y = y;
    tml::EventSystem::GetInstance().PushEvent(event);
    glViewport(0, 0, x, y);
}

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
        #ifdef TML_USE_GLES
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

        #if PLATFORM_UNIX
            img.pixels = stbi_load_from_memory(
                gTML_ICONData,
                static_cast<int>(gTML_ICONSize),
                &img.width,
                &img.height,
                &channels,
                4);
        #else
            img.pixels = stbi_load_from_memory(
                LOGO_DATA.data(),
                static_cast<int>(LOGO_DATA.size()),
                &img.width,
                &img.height,
                &channels,
                4);
        #endif
        glfwSetWindowIcon(handle,1, &img);
        delete[] img.pixels;
    }

    Window::~Window()
    {
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_handle));
//        glfwTerminate();
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

    Event Window::PollEvents() const noexcept
    {
        return EventSystem::GetInstance().PollEvents();
    }

    Event Window::WaitEvents() const noexcept
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
                tml::Condition(user_w == -1, w, user_w),
                tml::Condition(user_h == -1, h, user_h),
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
        glfwSetCursorPosCallback(handle, MouseMoveCallback);
        glfwSetScrollCallback(handle, MouseScrollCallback);
        glfwSetWindowCloseCallback(handle, [](GLFWwindow* handle){
            Event event;
            event.type = Event::Closed;
            event.sender = handle;
            EventSystem::GetInstance().PushEvent(event);
        });
    }
}