#pragma once
#include <string>
#include <TML/Types.h>

namespace tml
{
    class Sound {
    public:
        enum State { Stopped, Playing, Paused, Ended };
    public:
        Sound();
        Sound(const std::string &filename);
        ~Sound();
        bool LoadFromFile(const std::string& filename);
        bool LoadFromData(void* data, ui32 bytes);
        void Play();
        void Stop();
        void Pause();
        void Resume();
        void SetLooping(bool loop);
        void SetVolume(float volume);
        float GetVolume() const { return m_volume; }
        bool IsPlaying() const { return m_state == Playing; }
        bool IsLooping() const { return m_looping; }
        void* GetDecoder() { return m_decoder; }
    private:
        ui32 m_id;
        void* m_decoder;
        State m_state = Stopped;
        float m_pitch = 1.f;
        float m_speed = 1.f;
        float m_volume = 1.f;
        bool m_looping = false;
        static ui32 s_soundCount;
    };
};