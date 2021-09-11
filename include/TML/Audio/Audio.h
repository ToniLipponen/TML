#pragma once
#include <string>
#include "../Types.h"

namespace tml
{
    class Sound {
    public:
        enum State { Stopped, Playing, Paused };
    public:
        Sound();
        Sound(const std::string &filename);
        ~Sound();
        void Play();
        void Stop();
        void Pause();
        void Resume();
        void SetLooping(bool loop);
        void SetPitch(float pitch);
        void SetVolume(float volume);
        bool IsPlaying() const { return m_state == Playing; }

    private:
        void* m_decoder;
        State m_state = Stopped;
        float m_pitch = 1.f;
        float m_speed = 1.f;
        bool m_looping = false;
    };
};