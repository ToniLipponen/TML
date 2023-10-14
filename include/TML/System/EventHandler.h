#pragma once
#include <functional>
#include <vector>

namespace tml
{
    struct Event
    {
        bool handled = false;
    };

    template<typename Sender, typename EventType = Event>
    struct EventHandler
    {
        using Callback = std::function<void(Sender*, EventType&)>;

        EventHandler& operator=(const EventHandler& other) = default;

        EventHandler& operator=(EventHandler&& other) noexcept = default;

        EventHandler& operator=(const Callback& callback)
        {
            Assign(callback);

            return *this;
        }

        EventHandler& operator+=(const Callback& callback)
        {
            Register(callback);

            return *this;
        }

        void Invoke(Sender* sender, EventType& args) const
        {
            for(auto& callback : m_callbacks)
            {
                callback(sender, args);

                if(args.handled)
                    break;
            }
        }

        void Invoke(Sender* sender) const
        {
            EventType e{};
            Invoke(sender, e);
        }

        void Register(const Callback& callback)
        {
            m_callbacks.push_back(callback);
        }

        void Assign(const Callback& callback)
        {
            Clear();
            Register(callback);
        }

        void Clear()
        {
            m_callbacks.clear();
        }
    private:
        std::vector<Callback> m_callbacks;
    };
}