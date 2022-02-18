#include <TML/Window/Event.h>
#include <TML/Window/Input.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <thread>
#include <chrono>

namespace tml
{
    EventSystem* EventSystem::m_instance = nullptr;

    EventSystem::EventSystem()
    {

    }

    EventSystem& EventSystem::GetInstance()
    {
        if(m_instance == nullptr)
            m_instance = new EventSystem();
        return *m_instance;
    }

    Event EventSystem::PollEvents() noexcept
    {
        glfwPollEvents();
        PollMouse();
        return PopEvent();
    }

    Event EventSystem::WaitEvents() noexcept
    {
        Event event = PollEvents();
        if(event.type == Event::Null)
        {
            do
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                event = PollEvents();
            }while(event.type == Event::EventType::Null);
        }
        return PopEvent();
    }

    void EventSystem::PushEvent(Event &event)
    {
        m_eventQueue.push(event);
    }

    Event EventSystem::PopEvent() noexcept
    {
        Event event{};
        if(m_eventQueue.empty())
        {
            return event;
        }
        event = m_eventQueue.front();
        m_eventQueue.pop();
        return event;
    }

    void EventSystem::PollMouse()
    {
        static double x = 0, y = 0;
        double oldX = x, oldY = y;
        glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);

        if(x != oldX || y != oldY)
        {
            Event event;
            event.type = Event::EventType::MouseMoved;
            event.mouseMove.x = x;
            event.mouseMove.y = y;
            PushEvent(event);
        }

        static i32 buttonState[7];

        for(ui32 i = 0; i < 7; i++)
        {
            const i32 oldButtonState = buttonState[i];
            buttonState[i] = glfwGetMouseButton(glfwGetCurrentContext(), i);
            if(oldButtonState != buttonState[i])
            {
                Event event;
                event.type = (buttonState[i] == GLFW_PRESS) ? Event::EventType::MouseButtonPressed : Event::EventType::MouseButtonReleased;
                event.mouseButton.button = i;
                event.mouseButton.x = x;
                event.mouseButton.y = y;
                PushEvent(event);
            }
        }
    }
}







