#include <TML/Audio/AudioStream.h>
#include <TML/System/Math.h>

namespace tml
{
    AudioStream::AudioStream() noexcept
    {
        m_channels = 2;
        m_rate = 48000;
        m_valid = true;
        m_looping = true;
    }

    AudioStream& AudioStream::operator<<(const AudioBuffer& rhs) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);

        auto& buffer = rhs.GetData();
        m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.end());
        return *this;
    }

    AudioBuffer& AudioStream::operator>>(AudioBuffer& rhs) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);

        auto& buffer = rhs.GetData();
        buffer.insert(buffer.end(), m_buffer.begin(), m_buffer.end());
        m_buffer.clear();

        return rhs;
    }

    uint64_t AudioStream::GetLength() noexcept
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        return m_buffer.size();
    }

    uint32_t AudioStream::ReadFrames(float *output, uint32_t frameCount)
    {
        std::lock_guard<std::mutex> scopedLock(m_mutex);

        const auto readFrames = Math::Min<uint32_t>(m_buffer.size(), frameCount * 2);
        auto left  = Math::Map<float>(m_balance,  1, 0, 0, 1);
        auto right = Math::Map<float>(m_balance, -1, 0, 0, 1);

        if(readFrames > 0)
        {
            for(uint32_t i = 0; i < readFrames; i += 2)
            {
                output[i  ] += m_buffer[i  ] * m_volume * left;
                output[i+1] += m_buffer[i+1] * m_volume * right;
            }

            m_buffer.erase(m_buffer.begin(), m_buffer.begin() + readFrames);
        }

        return frameCount / 2;
    }
}