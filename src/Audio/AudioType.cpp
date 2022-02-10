#include <TML/Audio/AudioType.h>
#include "Mixer.h"

namespace tml
{
    ui32 AudioType::s_soundCount = 0;
    static bool AudioInitialized = false;

    AudioType::AudioType()
    : m_framesRead(0), m_frameCount(0), m_rate(0), m_channels(0), m_id(s_soundCount++)
    {
        if(!AudioInitialized)
            AudioInitialized = Mixer::Init();
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
            m_state = Stopped;
        m_framesRead = 0;
    }

    void AudioType::Pause()
    {
        if(m_valid)
            m_state = Paused;
    }

    void AudioType::Resume()
    {
        if(m_valid)
           m_state = Playing;
    }

    void AudioType::SetLooping(bool loop)
    {
        m_looping = loop;
    }

    void AudioType::SetVolume(float volume)
    {
        m_volume = volume;
    }

    ui64 AudioType::GetLengthInSeconds() const noexcept
    {
        return m_frameCount / m_rate / m_channels; // Danger! m_rate or m_channels might be 0. TODO.
    }

    double AudioType::GetProgress() const noexcept
    {
        return static_cast<double>(m_framesRead) / static_cast<double>(m_frameCount);
    }
}