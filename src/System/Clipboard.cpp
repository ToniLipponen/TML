#include <TML/System/Clipboard.h>
#include <TML/System/File.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    bool Clipboard::IsEmpty()
    {
        glfwInit();
        return glfwGetClipboardString(nullptr) == nullptr;
    }

    void Clipboard::Clear()
    {
        glfwInit();
        glfwSetClipboardString(nullptr,"\0");
    }

    bool Clipboard::GetString(String& string)
    {
        glfwInit();

        if(!IsEmpty())
        {
            string = String(glfwGetClipboardString(nullptr));
            return true;
        }

        return false;
    }

    std::optional<String> Clipboard::GetString()
    {
        String text;

        if(GetString(text))
        {
            return text;
        }

        return std::nullopt;
    }

    void Clipboard::SetString(const String& string)
    {
        glfwInit();
        glfwSetClipboardString(nullptr,string.c_str());
    }
}