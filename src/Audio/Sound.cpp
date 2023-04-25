#include <TML/Audio/Sound.h>
#include <TML/System/Math.h>
#include <iostream>

namespace tml
{
    Sound::Sound(const String& filename)
    : m_buffer(std::make_shared<AudioBuffer>())
    {
        LoadFromFile(filename);
    }

    Sound::Sound(const void* data, size_t bytes)
    {
        LoadFromData(data, bytes);
    }

    Sound::Sound(const float *data, uint8_t channels, uint32_t rate, size_t sampleCount)
    : m_buffer(std::make_shared<AudioBuffer>())
    {
        LoadFromMemory(data, channels, rate, sampleCount);
    }

    Sound::Sound(const Sound& sound)
    {
        *this = sound;
    }

    Sound::Sound(Sound&& sound) noexcept
    {
        *this = sound;
        sound.m_id = 0;
    }

    Sound& Sound::operator=(const Sound& sound) noexcept
    {
        if(&sound == this)
        {
            return *this;
        }

        m_framesRead    = 0;
        m_buffer        = sound.m_buffer;
        m_frameCount    = sound.m_frameCount;
        m_volume        = sound.m_volume;
        m_looping       = sound.m_looping;
        m_valid         = sound.m_valid;
        m_state         = Stopped;
        return *this;
    }

    bool Sound::LoadFromFile(const String& filename) noexcept
    {
        m_state = Stopped;
        m_buffer = std::make_shared<AudioBuffer>(filename);
        m_valid = !m_buffer->GetData().empty();
        m_frameCount = m_buffer->GetData().size();
        m_framesRead = 0;
        return m_valid;
    }

    bool Sound::LoadFromData(const void *data, size_t bytes) noexcept
    {
        m_state = Stopped;
        m_buffer = std::make_shared<AudioBuffer>(data, bytes);
        m_valid = !m_buffer->GetData().empty();
        m_frameCount = m_buffer->GetData().size();
        m_framesRead = 0;
        return m_valid;
    }

    bool Sound::LoadFromMemory(const float *data, uint8_t channels, uint32_t rate, size_t sampleCount) noexcept
    {
        m_state = Stopped;
        m_buffer = std::make_shared<AudioBuffer>(data, channels, rate, sampleCount);
        m_valid = !m_buffer->GetData().empty();
        m_frameCount = m_buffer->GetData().size();
        m_framesRead = 0;
        return m_valid;
    }

    void Sound::SetBuffer(const AudioBuffer& buffer) noexcept
    {
        m_buffer = std::make_shared<AudioBuffer>(buffer);
        m_valid = !m_buffer->GetData().empty();
        m_frameCount = m_buffer->GetData().size();
        m_framesRead = 0;
    }

    void Sound::SetBuffer(const std::shared_ptr<AudioBuffer>& buffer) noexcept
    {
        m_buffer = buffer;

        if(buffer == nullptr)
        {
            m_valid = false;
            return;
        }

        m_valid = !m_buffer->GetData().empty();
        m_frameCount = m_buffer->GetData().size();
        m_framesRead = 0;
    }

    uint32_t Sound::ReadFrames(AudioFrame* output, uint32_t frameCount)
    {
        if(m_buffer == nullptr)
        {
            return 0;
        }

        std::array<AudioFrame, 2048> buffer;
        const auto framesRead = Math::Clamp<uint64_t>(frameCount, 0, Math::Min<uint64_t>(m_frameCount - m_framesRead, buffer.size()));

        AudioFrame balance = {
            Math::Map<float>(m_balance, 1, 0, 0, 1),
            Math::Map<float>(m_balance, -1, 0, 0, 1)
        };

        for(uint32_t i = 0; i < framesRead; i++)
        {
            buffer[i] = m_buffer->GetData()[m_framesRead + i];
        }

        for(auto& [name, effect] : m_effects)
        {
            if(effect->IsEnabled())
            {
                for(int i = 0; i < framesRead; i++)
                {
                    effect->Process(buffer.data() + i, m_framesRead + i, m_frameCount);
                }
            }
        }

        for(uint32_t i = 0; i < framesRead; i++)
        {
            output[i] += (buffer[i] * m_volume * balance);
        }

        m_framesRead += framesRead;

        return framesRead;
    }
}