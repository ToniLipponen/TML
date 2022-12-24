#include <TML/Audio/AudioStream.h>
#include <TML/System/Math.h>
#include <iostream>

namespace tml
{
    AudioStream::AudioStream() noexcept
    {
        m_channels = 2;
        m_rate = 48000;
        m_valid = true;
        m_looping = true;
    }

    AudioStream& AudioStream::operator<<(AudioStream& rhs) noexcept
    {
        std::lock_guard<std::mutex> lockGuard1(m_mutex);
        std::lock_guard<std::mutex> lockGuard2(rhs.m_mutex);

        auto& target = m_buffer;
        auto& source = rhs.m_buffer;

        auto inputLength = Math::Min(m_bufferLength - target.size(), source.size());
        target.insert(target.end(), source.begin(), source.begin() + inputLength);
        source.erase(source.begin(), source.begin() + inputLength);

        return *this;
    }

    AudioStream& AudioStream::operator<<(const AudioBuffer& rhs) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);

        if(!IsBufferFull())
        {
            auto inputLength = static_cast<std::deque<float>::difference_type>(m_bufferLength - m_buffer.size());
            auto& buffer = rhs.GetData();
            m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.begin() + inputLength);
        }

        return *this;
    }

    AudioStream& AudioStream::operator>>(AudioBuffer& rhs) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);

        auto& buffer = rhs.GetData();
        buffer.insert(buffer.end(), m_buffer.begin(), m_buffer.end());
        m_buffer.clear();

        return *this;
    }

    AudioStream& AudioStream::operator>>(AudioStream& rhs) noexcept
    {
        return rhs << (*this);
    }

    uint64_t AudioStream::GetLength() noexcept
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        return m_buffer.size();
    }

    void AudioStream::Flush() noexcept
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        m_buffer.clear();
    }

    size_t AudioStream::SetBufferLength(size_t length) noexcept
    {
        std::lock_guard<std::mutex> lockGuard(m_mutex);
        m_buffer.resize(length);
        return m_bufferLength = length;
    }

    size_t AudioStream::GetBufferLength() const noexcept
    {
        return m_bufferLength;
    }

    bool AudioStream::IsBufferFull() noexcept
    {
        return m_buffer.size() >= m_bufferLength;
    }

    bool AudioStream::IsBufferEmpty() noexcept
    {
        return m_buffer.empty();
    }

    void AudioStream::SetOnReadCallback(const std::function<void(AudioStream&, uint32_t, void*)>& callback, void* userData) noexcept
    {
        m_callback = callback;
        m_userData = userData;
    }

    uint32_t AudioStream::ReadFrames(float *output, uint32_t frameCount)
    {
        uint32_t readSamples;

        {
            std::lock_guard<std::mutex> scopedLock(m_mutex);
            readSamples = Math::Min<uint32_t>(m_buffer.size(), frameCount * 2);
            auto left  = Math::Map<float>(m_balance,  1, 0, 0, 1);
            auto right = Math::Map<float>(m_balance, -1, 0, 0, 1);

            if(readSamples > 0)
            {
                for(uint32_t i = 0; i < readSamples; i += 2)
                {
                    output[i  ] += m_buffer[i  ] * m_volume * left;
                    output[i+1] += m_buffer[i+1] * m_volume * right;
                }

                m_buffer.erase(m_buffer.begin(), m_buffer.begin() + readSamples);
            }
        }

        m_callback(*this, m_bufferLength - m_buffer.size(), m_userData);

        return readSamples / 2;
    }
}