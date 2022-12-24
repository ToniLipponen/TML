#pragma once
#include <TML/System/NonCopyable.h>
#include <TML/Export.h>
#include <deque>
#include <mutex>

namespace tml
{
    template <typename T>
    class TML_API Queue : NonCopyable
    {
    public:
        Queue() = default;

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

        const T& Push(const T& value) noexcept
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push_back(value);

			return m_queue.back();
        }

		void Push(T&& value) noexcept
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.emplace_back(value);

			return m_queue.back();
		}

    private:
        std::deque<T> m_queue;
        std::mutex m_mutex;
    };
}