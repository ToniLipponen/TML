#pragma once
#include <TML/Types.h>

namespace tml
{
    class AudioType
    {
    public:
        enum State { Stopped, Playing, Paused, Ended };
    public:
        AudioType();
        ~AudioType();
        virtual void Play();
        virtual void Stop();
        virtual void Pause();
        virtual void Resume();
        void SetLooping(bool loop);
        void SetVolume(float volume);
        float GetVolume() const { return m_volume; }
        bool IsPlaying() const { return m_state == Playing; }
        bool IsLooping() const { return m_looping; }
        virtual ui32 ReadFrames(float* output, ui32 frameCount) = 0;
        friend void data_callback(struct ma_device*, void*, const void*, ui32);
    protected:
        float m_volume = 1.f;
        bool m_looping = false;
        State m_state = Stopped;
        static ui32 s_soundCount;
        ui32 m_id;
    };
}