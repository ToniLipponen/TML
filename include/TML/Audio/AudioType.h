#pragma once
#include "TML/Types.h"

namespace tml
{
    /* TODO: Clean this up. Put all function implementations in AudioType.cpp. */
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

        inline constexpr ui64 GetLength() const noexcept { return m_frameCount; } // @brief Returns length in PCM frames.
        ui64 GetLengthInSeconds() const noexcept;
        double GetProgress() const noexcept;
    protected:
        ui64 m_framesRead;
        ui64 m_frameCount;
        ui32 m_rate;
        ui8 m_channels;
        float m_volume = 1.f;
        bool m_looping = false;
        bool m_valid = false;
        State m_state = Stopped;
        static ui32 s_soundCount;
        ui32 m_id;
    };
}