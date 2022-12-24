#include <TML/System/Clipboard.h>
#include <TML/System/File.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <regex>

namespace tml
{
    bool Clipboard::IsEmpty()
    {
        glfwInit();
        return glfwGetClipboardString(nullptr) == nullptr;
    }

    /**
     * Using regex to check if clipboard string contains an image file extension.
     * Then checking if a file exists with that name.
     */
    bool Clipboard::HasImage()
    {
        glfwInit();
        const static std::regex regex("[^\\s]+(.*?)\\.(svg|jpg|jpeg|png|gif|SVG|JPG|JPEG|PNG|GIF)$");

        if(auto* clipboardString = glfwGetClipboardString(nullptr))
        {
            String str(clipboardString);

            return std::regex_match(str.cpp_str(), regex) && File::Exists(str.cpp_str());
        }

        return false;
    }

    bool Clipboard::HasText()
    {
        return !IsEmpty() && !HasImage();
    }

    bool Clipboard::GetString(String& string)
    {
        glfwInit();
        if(HasText())
        {
            string = String(glfwGetClipboardString(nullptr));
            return true;
        }

        return false;
    }

    bool Clipboard::GetImage(Image& image)
    {
        if(HasImage())
        {
            return image.LoadFromFile(glfwGetClipboardString(nullptr));
        }

        return false;
    }

    void Clipboard::Clear()
    {
        glfwInit();
        glfwSetClipboardString(nullptr,"\0");
    }

    void Clipboard::SetString(const String& string)
    {
        glfwInit();
        glfwSetClipboardString(nullptr,string.c_str());
    }
}