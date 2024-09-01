#include <TML/Audio/AudioType.h>
#include <TML/Audio/Mixer.h>
#include <TML/System/Math.h>
#include "MixerPriv.h"

namespace tml
{
    AudioType::AudioType()
    : m_framesRead(0),
      m_frameCount(0),
      m_rate(0),
      m_channels(0),
      m_id(Mixer::GetAudioID())
    {

    }

    AudioType::~AudioType()
    {
        Mixer::RemoveSound(m_id);
    }

    void AudioType::Play()
    {
        if(m_valid)
        {
            m_state = Playing;
            Mixer::AddSound(m_id, this);
        }

        m_framesRead = 0;
    }

    void AudioType::Stop()
    {
        if(m_valid)
        {
            m_state = Stopped;
        }

        m_framesRead = 0;

        tml::Event e;
        OnAudioEnd.Invoke(this, e);
    }

    void AudioType::Pause()
    {
        if(m_valid)
        {
            m_state = Paused;
        }
    }

    void AudioType::Resume()
    {
        if(m_valid)
        {
           m_state = Playing;
        }
    }

    void AudioType::SetLooping(bool loop)
    {
        m_looping = loop;
    }

    void AudioType::SetVolume(float volume)
    {
        m_volume = volume;
    }

    void AudioType::SetBalance(float balance)
    {
        m_balance = Math::Clamp<float>(balance, -1, 1);
    }

    float AudioType::GetVolume() const
    {
        return m_volume;
    }

    AudioType::State AudioType::GetState() const
    {
        return m_state;
    }

    bool AudioType::IsPlaying() const
    {
        return m_state == Playing;
    }

    bool AudioType::IsLooping() const
    {
        return m_looping;
    }

    uint64_t AudioType::GetLength() noexcept
    {
        return m_frameCount;
    }

    uint64_t AudioType::GetLengthInSeconds() noexcept
    {
        if((m_rate == 0) || (m_channels == 0))
        {
            return 0;
        }

        return GetLength() / m_rate / m_channels;
    }

    double AudioType::GetProgress() const noexcept
    {
        return static_cast<double>(m_framesRead) / static_cast<double>(m_frameCount);
    }
}