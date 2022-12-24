#pragma once
#include <TML/Window/Event.h>
#include <TML/System/NonCopyable.h>

namespace tml
{
    class TML_API EventSystem : NonCopyable
    {
    private:
        EventSystem() noexcept = default;

    public:
        static EventSystem& GetInstance() noexcept;

        bool Register(const void* handle) noexcept;          //!< Register new sender / emitter.
        bool Remove(const void* handle) noexcept;            //!< Remove a sender / emitter.
        bool PollEvents(const void*, Event& e) noexcept;     //!< Checks if there is events in the event queue.
        bool WaitEvents(const void*, Event& e) noexcept;     //!< Blocks until there is an event in the event queue.
        void PushEvent(const void*, Event& event) noexcept;  //!< Pushed new event to the event queue.
        void PushGlobalEvent(Event& event) noexcept;         //!< Push an event to all event queues.
        bool PopEvent(const void*, Event& e) noexcept;       //!< Get event from event queue. Returns true if popped new event.
        bool PeekEvent(const void*, Event& e) noexcept;      //!< Get the first event in the queue. (Doesn't remove the event from the queue).

    private:
        std::map<const void*, std::queue<Event>> m_handles;
    };
}