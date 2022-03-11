#include <TML/Audio/Sound.h>
#include <TML/System/Math.h>
#include <miniaudio/miniaudio.h>
#include <cstring>
#include <_Assert.h>
#include "TML/Audio/Mixer.h"

namespace tml
{
    extern ma_decoder_config s_DecoderConfig;
    Sound::Sound(const std::string &filename)
    : m_samples(nullptr)
    {
        LoadFromFile(filename);
    }

    Sound::Sound(const float *data, ui8 channels, ui64 sampleCount)
    : m_samples(nullptr)
    {
        LoadFromMemory(data, channels, sampleCount);
    }

    Sound::Sound(const Sound& sound)
    : m_samples(nullptr)
    {
        *this = sound;
    }

    Sound::Sound(Sound&& sound) noexcept
    : m_samples(nullptr)
    {
        *this = sound;
    }

    Sound::~Sound()
    {
        Mixer::GetInstance().RemoveSound(m_id);
        delete[] m_samples;
    }

    Sound& Sound::operator=(const Sound& sound) noexcept
    {
        if(&sound == this)
            return *this;

        delete[] m_samples;
        m_samples = new float[sound.m_frameCount];
        std::memcpy(m_samples, sound.m_samples, sound.m_frameCount);

        m_framesRead    = 0;
        m_frameCount    = sound.m_frameCount;
        m_rate          = sound.m_rate;
        m_channels      = sound.m_channels;
        m_volume        = sound.m_volume;
        m_looping       = sound.m_looping;
        m_valid         = sound.m_valid;
        m_state         = sound.m_state;
        return *this;
    }

    Sound& Sound::operator=(Sound&& sound) noexcept
    {
        m_samples = sound.m_samples;
        sound.m_samples = nullptr;

        m_framesRead    = 0;
        m_frameCount    = sound.m_frameCount;
        m_rate          = sound.m_rate;
        m_channels      = sound.m_channels;
        m_volume        = sound.m_volume;
        m_looping       = sound.m_looping;
        m_valid         = sound.m_valid;
        m_state         = sound.m_state;
        return *this;
    }

    bool Sound::LoadFromFile(const std::string &filename) noexcept
    {
        m_state = Stopped;
        Mixer::GetInstance().RemoveSound(m_id);

        static ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
        ma_decoder decoder;
        ma_result result = ma_decoder_init_file(filename.c_str(), &config, &decoder);
        m_valid = (result == MA_SUCCESS);
        if(!m_valid)
        {
            tml::Logger::ErrorMessage("Failed to load sound file -> %s", filename.c_str());
            return false;
        }
        m_frameCount = ma_decoder_get_length_in_pcm_frames(&decoder) * decoder.outputChannels;

        delete[] m_samples;
        m_samples = new float[m_frameCount];
        ma_decoder_read_pcm_frames(&decoder, m_samples, m_frameCount);

        m_framesRead = 0;
        m_rate = decoder.outputSampleRate;
        m_channels = decoder.outputChannels;

        return true;
    }

    bool Sound::LoadFromData(const void *data, ui64 bytes) noexcept
    {
        m_state = Stopped;
        Mixer::GetInstance().RemoveSound(m_id);
        static ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
        ma_decoder decoder;
        ma_result result = ma_decoder_init_memory(data, bytes, &config, &decoder);
        m_valid = (result == MA_SUCCESS);
        if (!m_valid)
        {
            tml::Logger::ErrorMessage("Failed to load sound");
            return false;
        }
        m_frameCount = ma_decoder_get_length_in_pcm_frames(&decoder) * decoder.outputChannels;

        delete[] m_samples;
        m_samples = new float[m_frameCount];
        ma_decoder_read_pcm_frames(&decoder, m_samples, m_frameCount);
        m_framesRead = 0;
        m_rate = decoder.outputSampleRate;
        m_channels = decoder.outputChannels;

        ma_decoder_uninit(&decoder);
        return true;
    }

    bool Sound::LoadFromMemory(const float *data, ui8 channels, ui64 sampleCount) noexcept
    {
        delete[] m_samples;
        m_samples = new float[sampleCount];
        std::memcpy(m_samples, data, sampleCount * sizeof(float));
        m_frameCount = sampleCount;
        m_framesRead = 0;
        m_channels = channels;
        m_valid = true;
        return true;
    }

    ui32 Sound::ReadFrames(float *output, ui32 frameCount)
    {
        const ui32 readFrames = Math::Clamp<ui32>(frameCount * m_channels, 0, m_frameCount - m_framesRead);

        for(ui32 i = 0; i < readFrames; ++i)
            output[i] += m_samples[m_framesRead + i] * m_volume;

        m_framesRead += readFrames;
        return frameCount;
    }

}