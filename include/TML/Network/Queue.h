#pragma once
#include <deque>
#include <mutex>

namespace tml
{
    namespace Net
    {
        template <typename T>
        class TML_API Queue
        {
        public:
            Queue() = default;
            Queue(const Queue<T>&) = delete;

            const T& Front() noexcept
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.front();
            }

            size_t Length() noexcept
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_queue.size();
            }

            T Pop() noexcept
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                const auto value = std::move(m_queue.front());
                m_queue.pop_front();
                return value;
            }

            void Push(const T& value) noexcept
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.push_back(value);
            }

        private:
            std::deque<T> m_queue;
            std::mutex m_mutex;
        };
    }
}