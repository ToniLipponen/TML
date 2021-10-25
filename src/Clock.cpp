#include <TML/Clock.h>

namespace tml
{
    Clock::Clock(){}
    double Clock::Reset() noexcept
    {
        const auto new_begin = std::chrono::high_resolution_clock::now();
        const double time = std::chrono::duration_cast<std::chrono::duration<double>>(new_begin - m_begin).count();
        m_begin = new_begin;
        return time;
    }

    double Clock::GetTime() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }
}