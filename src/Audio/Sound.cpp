#include <TML/Audio/Sound.h>
#include <miniaudio/miniaudio.h>
#include "Mixer.h"
#include "../internal/Assert.h"
#include <fstream>

namespace tml {
    extern ma_decoder_config s_decoder_config;
    volatile bool AudioInitialized = Mixer::Init();
    ui32 Sound::s_soundCount = 0;
    Sound::Sound()
    {
        m_decoder = new ma_decoder;
        m_id = s_soundCount++;
    }

    Sound::Sound(const std::string &filename)
    {
        m_decoder = new ma_decoder;
        ma_result result = ma_decoder_init_file(filename.c_str(), &s_decoder_config, (ma_decoder*)m_decoder);
        if (result != MA_SUCCESS)
            tml::Logger::ErrorMessage("Failed to load sound file -> %s", filename.c_str());
        m_id = s_soundCount++;
    }

    bool Sound::LoadFromFile(const std::string &filename)
    {
        ma_result result = ma_decoder_init_file(filename.c_str(), &s_decoder_config, (ma_decoder*)m_decoder);
        if (result != MA_SUCCESS)
        {
            tml::Logger::ErrorMessage("Failed to load sound file -> %s", filename.c_str());
            return false;
        }
        return true;
    }

    Sound::~Sound()
    {
        ma_decoder_uninit((ma_decoder*)m_decoder);
        delete (ma_decoder*)m_decoder;
    }

    void Sound::Play()
    {
        m_state = Playing;
        Mixer::AddSound(m_id, this);
    }

    void Sound::Stop() {
        ma_decoder_seek_to_pcm_frame((ma_decoder*)m_decoder, 0);
        m_state = Stopped;
    }

    void Sound::Pause() {
        m_state = Paused;
    }

    void Sound::Resume() {
        m_state = Playing;
    }

    void Sound::SetLooping(bool loop) {
        m_looping = loop;
    }

    void Sound::SetPitch(float pitch) {

    }

    void Sound::SetVolume(float volume) {
        m_volume = volume;
    }
};