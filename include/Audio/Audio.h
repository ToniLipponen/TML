#pragma once
#include <string>
#include "../Types.h"
#include "AudioBuffer.h"


class Sound
{
public:
    Sound();
    Sound(const std::string& filename);
    ~Sound();
    void Play();
    void Stop();
    void SetLoop(bool loop);
    void SetPitch(float pitch);
    void SetVolume(float volume);
private:
    float m_pitch = 1.f;
    float m_speed = 1.f;
    AudioBuffer m_buffer;
    ui32 m_id;
};

namespace Audio
{
    void Init();

    void Close();
};