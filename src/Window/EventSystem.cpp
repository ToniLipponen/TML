#include <TML/Window/EventSystem.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include <mutex>

static std::mutex s_mutex;

namespace tml
{
    EventSystem& EventSystem::GetInstance() noexcept
    {
        static EventSystem instance;

        return instance;
    }

    bool EventSystem::Register(const void* handle) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(s_mutex);

        const auto preSize = m_handles.size();
        m_handles[handle] = std::queue<Event>();
        
        return preSize < m_handles.size();
    }

    bool EventSystem::Remove(const void* handle) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(s_mutex);

        return m_handles.erase(handle) > 0;
    }

    bool EventSystem::PollEvents(const void* handle, Event& event) noexcept
    {
        if(m_handles.find(handle) == m_handles.end())
        {
            return false;
        }

        glfwPollEvents();

        return PopEvent(handle, event);
    }

    bool EventSystem::WaitEvents(const void* handle, Event& e) noexcept
    {
        if(m_handles.find(handle) == m_handles.end())
        {
            return false;
        }

        /// Poll events until there is an event.
        while(!PollEvents(handle, e))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(8));
        }

        return true;
    }

    void EventSystem::PushEvent(const void* handle, Event &event) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(s_mutex);

        if(m_handles.find(handle) != m_handles.end())
        {
            m_handles.at(handle).push(event);
        }
    }

    void EventSystem::PushGlobalEvent(Event& event) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(s_mutex);

        for(auto& [handle, queue] : m_handles)
        {
            queue.push(event);
        }
    }

    bool EventSystem::PopEvent(const void* handle, Event& e) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(s_mutex);

        auto& queue = m_handles.at(handle);

        if(queue.empty())
        {
            return false;
        }

        e = queue.front();
        queue.pop();

        return true;
    }

    bool EventSystem::PeekEvent(const void* handle, Event& e) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(s_mutex);

        auto& queue = m_handles.at(handle);

        if(queue.empty())
        {
            return false;
        }

        e = queue.front();

        return true;
    }
}







