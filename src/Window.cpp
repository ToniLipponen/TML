#include "../include/Window.h"
#include "../external-headers/glad/glad.h"
#include "../external-headers/GLFW/glfw3.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "../external-headers/stb/stb_image_write.h"
#include <iostream>

void WindowResizeCallback(GLFWwindow* f, int x, int y)
{
    glViewport(0, 0, x, y);
}
namespace tml {
    Window::Window(ui32 w, ui32 h, cstring title, ui32 settings)
            : m_handle(nullptr) {
        if (glfwInit() == GLFW_FALSE) {
            std::cerr << "ERROR: Failed to initialize window.\n";
            std::exit(1);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, (settings & Settings::Resizeable) != 0);
        glfwWindowHint(GLFW_MAXIMIZED, (settings & Settings::Maximized) != 0);
        glfwWindowHint(GLFW_DOUBLEBUFFER, (settings & Settings::VSync) != 0);
        glfwWindowHint(GLFW_SAMPLES, (settings & Settings::Antialias) * 8);

        if (settings & Settings::Fullscreen)
            m_handle = glfwCreateWindow(w, h, title, glfwGetPrimaryMonitor(), 0);
        else
            m_handle = glfwCreateWindow(w, h, title, 0, 0);
        if (!m_handle) {
            std::cerr << "ERROR: Failed to create a window.\n";
            std::exit(2);
        }
        glfwMakeContextCurrent(reinterpret_cast<GLFWwindow *>(m_handle));
        glfwShowWindow(reinterpret_cast<GLFWwindow *>(m_handle));

        glfwSetWindowSizeCallback(reinterpret_cast<GLFWwindow *>(m_handle), WindowResizeCallback);
        Keyboard::Initialize();
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
        glfwSetWindowSize(reinterpret_cast<GLFWwindow *>(m_handle), w, h);
    }

    void Window::SetTitle(cstring title) {
        glfwSetWindowTitle(reinterpret_cast<GLFWwindow *>(m_handle), title);
    }

    void Window::Maximize() {
        glfwMaximizeWindow(reinterpret_cast<GLFWwindow *>(m_handle));
    }

    void Window::Screenshot(const cstring filename) {
        const ui32 w = GetWidth(), h = GetHeight();
        ui8* pixels = new ui8[3 * w * h];
        glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        stbi_flip_vertically_on_write(1);
        stbi_write_png(filename, w, h, 3, pixels, 0);
        delete[] pixels;
    }
};