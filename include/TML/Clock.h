#pragma once
#include <chrono>

namespace tml
{
    class Clock
    {
    public:
        Clock();
        double Reset() noexcept; // @brief Resets clock and returns time.
        double GetTime() const noexcept; // @brief Returns time.
    private:
        std::chrono::time_point<std::chrono::system_clock> m_begin;
    };
}