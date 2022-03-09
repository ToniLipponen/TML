#include <TML/Window/Event.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <thread>
#include <chrono>

namespace tml
{
    EventSystem* EventSystem::m_instance = nullptr;

    EventSystem::EventSystem() = default;

    EventSystem& EventSystem::GetInstance()
    {
        if(m_instance == nullptr)
            m_instance = new EventSystem();
        return *m_instance;
    }

    bool EventSystem::PollEvents(Event& event) noexcept
    {
        glfwPollEvents();
        return PopEvent(event);
    }

    bool EventSystem::WaitEvents(Event& e) noexcept
    {
        /// Poll events until there is an event.
        while(!PollEvents(e))
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        return true;
    }

    void EventSystem::PushEvent(Event &event)
    {
        m_eventQueue.push(event);
    }

    bool EventSystem::PopEvent(Event& e) noexcept
    {
        if(m_eventQueue.empty())
            return false;

        e = m_eventQueue.front();
        m_eventQueue.pop();
        return true;
    }
}







