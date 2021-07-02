#include "../../include/Audio/Audio.h"
#include "../../include/Audio/Wav.h"
#include "../../include/Assert.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <cstring>
#include <fstream>

ALCdevice* audioDevice;
ALCcontext* audioContext;
ALenum audioFormat;

Sound::Sound()
{
    alGenSources(1, &m_id);    
    alSourcef(m_id, AL_PITCH, 1);
    alSourcef(m_id, AL_GAIN, 1.0f);
    alSource3f(m_id, AL_POSITION, 0, 0, 0);
    alSource3f(m_id, AL_VELOCITY, 0, 0, 0);
    alSourcei(m_id, AL_LOOPING, AL_FALSE);
    alSourcei(m_id, AL_BUFFER, m_buffer.m_id);
}

Sound::Sound(const std::string& filename)
{
    m_buffer.LoadFromFile(filename);
    alGenSources(1, &m_id);
    alSourcei(m_id, AL_BUFFER, m_buffer.m_id);
}

Sound::~Sound()
{
    alDeleteSources(1, &m_id);
}

void Sound::Play()
{
    alSourcePlay(m_id);
}

void Sound::Stop()
{
    alSourceStop(m_id);
}

void Sound::SetLoop(bool loop)
{
    alSourcei(m_id, AL_LOOPING, loop);
}

void Sound::SetPitch(float pitch)
{
    alSourcef(m_id, AL_PITCH, pitch);
}

void Sound::SetVolume(float volume)
{
    alSourcef(m_id, AL_GAIN, volume);
}

namespace Audio
{

    void Init()
    {
        const char * defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
        audioDevice = alcOpenDevice(defname);
        audioContext = alcCreateContext(audioDevice, nullptr);
        alcMakeContextCurrent(audioContext);
    }

    void Close()
    {
        alcMakeContextCurrent(0);
        alcDestroyContext(audioContext);
        alcCloseDevice(audioDevice);
    }
};