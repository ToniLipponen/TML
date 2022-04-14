#include <TML/Window/Event.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
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

    bool EventSystem::Register(const void* handle) noexcept
    {
        const auto preSize = m_handles.size();
        m_handles[handle] = std::queue<Event>();
        return preSize < m_handles.size();
    }

    bool EventSystem::Remove(const void* handle) noexcept
    {
        return m_handles.erase(handle) > 0;
    }

    bool EventSystem::PollEvents(const void* handle, Event& event) noexcept
    {
        if(m_handles.find(handle) == m_handles.end())
            return false;

        glfwPollEvents();
        return PopEvent(handle, event);
    }

    bool EventSystem::WaitEvents(const void* handle, Event& e) noexcept
    {
        if(m_handles.find(handle) == m_handles.end())
            return false;

        /// Poll events until there is an event.
        while(!PollEvents(handle, e))
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        return true;
    }

    void EventSystem::PushEvent(const void* handle, Event &event) noexcept
    {
        if(m_handles.find(handle) != m_handles.end())
            m_handles.at(handle).push(event);
    }

    bool EventSystem::PopEvent(const void* handle, Event& e) noexcept
    {
        auto& queue = m_handles.at(handle);
        if(queue.empty())
            return false;

        e = queue.front();
        queue.pop();
        return true;
    }
}







