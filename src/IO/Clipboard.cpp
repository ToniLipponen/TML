#include <TML/IO/Clipboard.h>
#include <GLFW/glfw3.h>
#include <string.h>
using namespace tml;

bool Clipboard::IsEmpty()
{
    return glfwGetClipboardString(glfwGetCurrentContext()) == nullptr;
}

std::wstring Clipboard::GetString()
{
    auto* str = glfwGetClipboardString(glfwGetCurrentContext());
    return {str, str+std::string(str).length()};
}

void Clipboard::Clear()
{
    glfwSetClipboardString(glfwGetCurrentContext(), "");
}

// Drag and drop
static std::vector<std::wstring> s_droppedFiles;

extern "C" void DragAndDropCallback(GLFWwindow* window, int count, const char* files[])
{
    s_droppedFiles.clear();
    for(int i = 0; i < count; i++)
        s_droppedFiles.push_back({files[i], files[i]+strlen(files[i])});
}

bool DragAndDrop::IsEmpty()
{
    return s_droppedFiles.empty();
}

void DragAndDrop::Clear()
{
    s_droppedFiles.clear();
}

const std::vector<std::wstring>& DragAndDrop::GetFiles()
{
    return s_droppedFiles;
}