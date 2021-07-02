#include "../include/Window.h"
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include "../include/Assert.h"
#include <iostream>

void WindowResizeCallback(GLFWwindow* f, int x, int y)
{
    glad_glViewport(0, 0, x, y);
}

void WindowClose(GLFWwindow* w, int a, int b, int c, int d)
{
    if(a == GLFW_KEY_ESCAPE)
    {
        glfwDestroyWindow(w);
        glfwTerminate();
    }
}

Window::Window(ui32 w, ui32 h, cstring title, ui32 settings)
: m_handle(nullptr)
{
    if(glfwInit() == GLFW_FALSE)
    {
        std::cerr << "ERROR: Failed to initialize window.\n";
        std::exit(1);
    }
    
    glfwWindowHint(GLFW_RESIZABLE,      (settings & Setting::Resizeable) != 0);
    glfwWindowHint(GLFW_MAXIMIZED,      (settings & Setting::Maximized)  != 0);
    glfwWindowHint(GLFW_DOUBLEBUFFER,   (settings & Setting::VSync)      != 0);

    if((settings & Setting::Antialias) != 0)
        glfwWindowHint(GLFW_SAMPLES, 4);

    m_handle = glfwCreateWindow(w, h, title, 0, 0);
    if(!m_handle)
    {
        std::cerr << "ERROR: Failed to create a window.\n";
        std::exit(2);
    }
    glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(m_handle));
    glfwShowWindow(reinterpret_cast<GLFWwindow*>(m_handle));
    
    glfwSetWindowSizeCallback(reinterpret_cast<GLFWwindow*>(m_handle), WindowResizeCallback);
    glfwSetKeyCallback(reinterpret_cast<GLFWwindow*>(m_handle), WindowClose);
}

Window::~Window()
{
    glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(m_handle));
    glfwTerminate();
}

void Window::Display()
{
	TL_ASSERT(glad_glGetError() == 0, "glError");
    glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(m_handle));
    glfwPollEvents();
}

bool Window::ShouldClose() const noexcept
{
    return static_cast<bool>(glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(m_handle)));
}

void* Window::GetHandle() noexcept
{
    return m_handle;
}

i32 Window::GetWidth() const noexcept
{
    i32 w, h;
    glfwGetWindowSize(reinterpret_cast<GLFWwindow*>(m_handle), &w, &h);
    return w;
}

i32 Window::GetHeight() const noexcept
{
    i32 w, h;
    glfwGetWindowSize(reinterpret_cast<GLFWwindow*>(m_handle), &w, &h);
    return h;
}

i32 Window::GetX() const noexcept
{
    i32 x, y;
    glfwGetWindowPos(reinterpret_cast<GLFWwindow*>(m_handle), &x, &y);
    return x;
}

i32 Window::GetY() const noexcept
{
    i32 x, y;
    glfwGetWindowPos(reinterpret_cast<GLFWwindow*>(m_handle), &x, &y);
    return y;
}

void Window::SetSize(ui32 w, ui32 h) noexcept
{
    // glfwMaximizeWindow(reinterpret_cast<GLFWwindow*>(m_handle));
    glfwRestoreWindow(reinterpret_cast<GLFWwindow*>(m_handle));
    glfwSetWindowSize(reinterpret_cast<GLFWwindow*>(m_handle), w, h);
}

void Window::SetTitle(cstring title)
{
    glfwSetWindowTitle(reinterpret_cast<GLFWwindow*>(m_handle), title);
}

void Window::Maximize()
{
    glfwMaximizeWindow(reinterpret_cast<GLFWwindow*>(m_handle));
}