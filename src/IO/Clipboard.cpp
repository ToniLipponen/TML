#include <TML/IO/Clipboard.h>
#include <GLFW/glfw3.h>

namespace tml
{
    // Drag and drop
    static std::vector<std::string> s_droppedFiles;

    bool Clipboard::IsEmpty()
    {
        return glfwGetClipboardString(glfwGetCurrentContext()) == nullptr;
    }

    std::string Clipboard::GetString()
    {
        return glfwGetClipboardString(glfwGetCurrentContext());
    }

    void Clipboard::Clear()
    {
        glfwSetClipboardString(glfwGetCurrentContext(), "");
    }

    extern "C" void DragAndDropCallback(GLFWwindow*, int count, const char* files[])
    {
        s_droppedFiles.clear();
        for(int i = 0; i < count; i++)
            s_droppedFiles.emplace_back(files[i]);
    }

    bool DragAndDrop::IsEmpty()
    {
        return s_droppedFiles.empty();
    }

    void DragAndDrop::Clear()
    {
        s_droppedFiles.clear();
    }

    const std::vector<std::string>& DragAndDrop::GetFiles()
    {
        return s_droppedFiles;
    }
}