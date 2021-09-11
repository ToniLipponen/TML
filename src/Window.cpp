#include "../include/TML/Window.h"
#include "internal/Assert.h"
#include <TML/Utilities/Condition.h>
#include "../external-headers/glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "../external-headers/GLFW/glfw3.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "../external-headers/stb/stb_image_write.h"
#include "../external-headers/stb/stb_image.h"
#include "internal/Default_icon.h"
#include "internal/Default_cursor.h"


void WindowResizeCallback(__attribute__((unused)) GLFWwindow* f, int x, int y)
{
    glViewport(0, 0, x, y);
}

namespace tml {
    Window::Window(i32 w, i32 h, cstring title, ui32 settings)
    : m_handle(nullptr), m_title(title)
    {
        TML_ASSERT(glfwInit(), "Failed to initialize window.");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, (settings & Settings::Resizeable) != 0);
        glfwWindowHint(GLFW_MAXIMIZED, (settings & Settings::Maximized) != 0);
        glfwWindowHint(GLFW_DOUBLEBUFFER, (settings & Settings::VSync) != 0);
        glfwWindowHint(GLFW_SAMPLES, static_cast<int>((settings & Settings::Antialias) * 4));

        if (settings & Settings::Fullscreen)
            m_handle = glfwCreateWindow(w, h, title, glfwGetPrimaryMonitor(), nullptr);
        else
            m_handle = glfwCreateWindow(w, h, title, nullptr, nullptr);
        TML_ASSERT(m_handle != nullptr, "Failed to create a window handle.");
        glfwMakeContextCurrent(reinterpret_cast<GLFWwindow *>(m_handle));
        glfwShowWindow(reinterpret_cast<GLFWwindow *>(m_handle));
        glfwSetWindowSizeCallback(reinterpret_cast<GLFWwindow *>(m_handle), WindowResizeCallback);
        Keyboard::Initialize();

        GLFWimage img, img2;
        int channels = 4;
        img.pixels = stbi_load_from_memory(
                DEFAULT_ICON_DATA.data(),
                static_cast<int>(DEFAULT_ICON_DATA.size()),
                &img.width,
                &img.height, &
                channels,
                4);
        glfwSetWindowIcon(reinterpret_cast<GLFWwindow*>(m_handle),1, &img);
        img2.pixels = stbi_load_from_memory(
                DEFAULT_CURSOR_DATA.data(),
                static_cast<int>(DEFAULT_CURSOR_DATA.size()),
                &img2.width,
                &img2.height,
                &channels,
                4);
        auto cursor = glfwCreateCursor(&img2, 0,0);
        glfwSetCursor(reinterpret_cast<GLFWwindow*>(m_handle), cursor);
        delete[] img.pixels;
        delete[] img2.pixels;
    }

    Window::~Window() {
        glfwDestroyWindow(reinterpret_cast<GLFWwindow *>(m_handle));
        glfwTerminate();
    }

    void Window::Display() {
        glfwSwapBuffers(reinterpret_cast<GLFWwindow *>(m_handle));
        glfwPollEvents();
    }

    void Window::Close() const noexcept {
        glfwSetWindowShouldClose(reinterpret_cast<GLFWwindow *>(m_handle), 1);
    }

    bool Window::ShouldClose() const noexcept {
        return static_cast<bool>(glfwWindowShouldClose(reinterpret_cast<GLFWwindow *>(m_handle)));
    }

    void *Window::GetHandle() noexcept {
        return m_handle;
    }

    i32 Window::GetWidth() const noexcept {
        i32 w, h;
        glfwGetWindowSize(reinterpret_cast<GLFWwindow *>(m_handle), &w, &h);
        return w;
    }

    i32 Window::GetHeight() const noexcept {
        i32 w, h;
        glfwGetWindowSize(reinterpret_cast<GLFWwindow *>(m_handle), &w, &h);
        return h;
    }

    i32 Window::GetX() const noexcept {
        i32 x, y;
        glfwGetWindowPos(reinterpret_cast<GLFWwindow *>(m_handle), &x, &y);
        return x;
    }

    i32 Window::GetY() const noexcept {
        i32 x, y;
        glfwGetWindowPos(reinterpret_cast<GLFWwindow *>(m_handle), &x, &y);
        return y;
    }

    double Window::GetTime() const noexcept {
        return glfwGetTime();
    }

    void Window::SetSize(ui32 w, ui32 h) noexcept {
        glfwRestoreWindow(reinterpret_cast<GLFWwindow *>(m_handle));
        glfwSetWindowSize(reinterpret_cast<GLFWwindow *>(m_handle), static_cast<int>(w), static_cast<int>(h));
    }

    void Window::SetTitle(cstring title) {
        glfwSetWindowTitle(reinterpret_cast<GLFWwindow *>(m_handle), title);
    }

    void Window::Maximize() {
        glfwMaximizeWindow(reinterpret_cast<GLFWwindow *>(m_handle));
    }

    // Fix this
    [[maybe_unused]] void Window::SetFullscreen(bool full, i32 user_w, i32 user_h)
    {
        int w = 0, h = 0, x = 0, y = 0;
        auto monitor = glfwGetPrimaryMonitor();
        // Destroy old handle
        glfwDestroyWindow(reinterpret_cast<GLFWwindow *>(m_handle));
        m_handle = nullptr;
        glfwGetMonitorWorkarea(monitor, &w, &h, &x, &y);
        m_handle = glfwCreateWindow(
                tml::Condition(user_w == -1, w, user_w),
                tml::Condition(user_h == -1, h, user_h),
                m_title.c_str(),
                0,
                0);
//        TML_ASSERT(m_handle != nullptr, "Failed to create a window handle.");
        glfwMakeContextCurrent(reinterpret_cast<GLFWwindow *>(m_handle));
//        glfwShowWindow(reinterpret_cast<GLFWwindow *>(m_handle));
        glfwSetWindowSizeCallback(reinterpret_cast<GLFWwindow *>(m_handle), WindowResizeCallback);
    }
    void Window::Screenshot(const cstring filename) {
        const i32 w = GetWidth(), h = GetHeight();
        ui8* pixels = new ui8[3 * w * h];
        glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        stbi_flip_vertically_on_write(1);
        stbi_write_png(filename, w, h, 3, pixels, 0);
        delete[] pixels;
    }
};