#include <TML/Window/Cursor.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    Cursor::Cursor(int shape) noexcept
    {
        glfwInit();
        m_handle = glfwCreateStandardCursor(shape);
    }

    Cursor::Cursor() noexcept
    {
        glfwInit();
    }

    [[maybe_unused]] Cursor::Cursor(const Image &image, int pointX, int pointY) noexcept
    {
        glfwInit();
        Create(image, pointX, pointY);
    }

    Cursor::~Cursor() noexcept
    {
        glfwDestroyCursor(static_cast<GLFWcursor*>(m_handle));
        m_handle = nullptr;
    }

    bool Cursor::Create(const Image& image, int pointX, int pointY) noexcept
    {
        GLFWimage glfwImage{};

        glfwImage.width = image.GetWidth();
        glfwImage.height = image.GetHeight();
        glfwImage.pixels = (uint8_t*)image.GetData();

        m_handle = glfwCreateCursor(&glfwImage, pointX, pointY);
        return m_handle != nullptr;
    }

    void* Cursor::GetHandle() const noexcept
    {
        return m_handle;
    }

    [[maybe_unused]] const Cursor Cursor::CrosshairCursor   = Cursor(GLFW_CROSSHAIR_CURSOR);
    [[maybe_unused]] const Cursor Cursor::IBeamCursor       = Cursor(GLFW_IBEAM_CURSOR);
    [[maybe_unused]] const Cursor Cursor::ArrowCursor       = Cursor(GLFW_ARROW_CURSOR);
    [[maybe_unused]] const Cursor Cursor::VResizeCursor     = Cursor(GLFW_VRESIZE_CURSOR);
    [[maybe_unused]] const Cursor Cursor::HResizeCursor     = Cursor(GLFW_HRESIZE_CURSOR);
    [[maybe_unused]] const Cursor Cursor::CenterCursor      = Cursor(GLFW_CENTER_CURSOR);
    [[maybe_unused]] const Cursor Cursor::HandCursor        = Cursor(GLFW_HAND_CURSOR);
    [[maybe_unused]] const Cursor Cursor::DefaultCursor     = Cursor();
}