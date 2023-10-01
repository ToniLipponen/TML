#pragma once
#include <TML/System/EventHandler.h>
#include <thread>

namespace tml
{
    class ScheduledEvent
    {
    public:
        ScheduledEvent(uint64_t milliseconds)
        : m_milliseconds(milliseconds)
        {

        }

        void Start()
        {
            m_thread = std::thread([&]()
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_milliseconds));
                tml::Event e;
                OnTimerExpired.Invoke(this, e);
            });

            m_thread.detach();
        }

        EventHandler<ScheduledEvent, tml::Event> OnTimerExpired;

    private:
        uint64_t m_milliseconds;
        std::thread m_thread;
    };
}