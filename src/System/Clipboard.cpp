#include <TML/System/Clipboard.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    bool Clipboard::IsEmpty()
    {
        return glfwGetClipboardString(glfwGetCurrentContext()) == nullptr;
    }

    String Clipboard::GetString()
    {
        return glfwGetClipboardString(glfwGetCurrentContext());
    }

    void Clipboard::Clear()
    {
        glfwSetClipboardString(glfwGetCurrentContext(), "");
    }

}