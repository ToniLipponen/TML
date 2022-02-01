#include <TML/IO/Clipboard.h>
#include <TML/IO/Event.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstring>

namespace tml
{
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

    extern "C" void DragAndDropCallback(GLFWwindow* window, int count, const char** files)
    {
        Event event;
        event.type = Event::FileDragAndDropped;
        event.dragAndDrop.count = count;
        event.dragAndDrop.paths = new char*[count];
        for(int i = 0; i < count; i++)
        {
            const auto str = std::string(files[i]);
            const auto len = str.size();

            event.dragAndDrop.paths[i] = new char[len];
            std::memcpy(event.dragAndDrop.paths[i], str.data(), len);
        }
        event.sender = window;
        EventSystem::GetInstance().PushEvent(event);
    }
}