#include <TML/Audio/AudioType.h>
#include <TML/Audio/Mixer.h>
#include <TML/System/Math.h>

namespace tml
{
    AudioType::AudioType()
    : m_framesRead(0),
      m_frameCount(0),
      m_rate(0),
      m_channels(0),
      m_id(Mixer::GetInstance().GetAudioID())
    {

    }

    AudioType::~AudioType()
    {
        Mixer::GetInstance().RemoveSound(m_id);
    }

    void AudioType::Play()
    {
        if(m_valid)
        {
            m_state = Playing;
            Mixer::GetInstance().AddSound(m_id, this);
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

    void AudioType::SetBalance(float balance)
    {
        m_balance = Math::Clamp<float>(balance, -1, 1);
    }

    ui64 AudioType::GetLengthInSeconds() const noexcept
    {
        if((m_rate == 0) || (m_channels == 0))
            return 0;

        return m_frameCount / m_rate / m_channels;
    }

    double AudioType::GetProgress() const noexcept
    {
        return static_cast<double>(m_framesRead) / static_cast<double>(m_frameCount);
    }
}