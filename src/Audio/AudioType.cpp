#include <TML/Audio/AudioType.h>
#include "Mixer.h"

namespace tml
{
    ui32 AudioType::s_soundCount = 0;
    AudioType::AudioType()
    {
        m_id = s_soundCount;
        s_soundCount++;
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
    }

    void AudioType::Stop()
    {
        if(m_valid)
            m_state = Stopped;
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

    void AudioType::SetLooping(bool loop) {
        m_looping = loop;
    }

    void AudioType::SetVolume(float volume) {
        m_volume = volume;
    }
}