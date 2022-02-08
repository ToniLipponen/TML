#include "TML/System/Clock.h"

namespace tml
{
    Clock::Clock() = default;
    double Clock::Reset() noexcept
    {
        const auto newBegin = std::chrono::high_resolution_clock::now();
        const double time = std::chrono::duration_cast<std::chrono::duration<double>>(newBegin - m_begin).count();
        m_begin = newBegin;
        return time;
    }

    double Clock::GetTime() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }
}