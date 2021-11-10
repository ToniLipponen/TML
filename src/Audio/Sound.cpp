#include <TML/Audio/Sound.h>
#include <miniaudio/miniaudio.h>
#include <TML/Utilities/Copy.h>
#include <TML/Utilities/Utilities.h>
#include "../internal/_Assert.h"
#include "Mixer.h"

namespace tml
{
    extern ma_decoder_config s_DecoderConfig;

    Sound::Sound()
    : m_samples(nullptr)
    {

    }

    Sound::Sound(const std::string &filename)
    : m_samples(nullptr)
    {
        LoadFromFile(filename);
    }

    Sound::Sound(const float *data, ui8 channels, ui64 sampleCount)
    :m_samples(nullptr)
    {
        LoadFromMemory(data, channels, sampleCount);
    }

    Sound::~Sound()
    {
        Mixer::RemoveSound(m_id);
        delete[] m_samples;
    }

    bool Sound::LoadFromFile(const std::string &filename)
    {
        ma_decoder decoder;
        ma_result result = ma_decoder_init_file(filename.c_str(), &s_DecoderConfig, &decoder);
        m_valid = (result == MA_SUCCESS);
        if (!m_valid)
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

        ma_decoder_uninit(&decoder);
        return true;
    }

    bool Sound::LoadFromData(const void *data, ui64 bytes)
    {
        ma_decoder decoder;
        ma_result result = ma_decoder_init_memory(data, bytes, &s_DecoderConfig, &decoder);
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

    bool Sound::LoadFromMemory(const float *data, ui8 channels, ui64 sampleCount)
    {
        delete[] m_samples;
        m_samples = new float[sampleCount];
        Util::Copy<float>(m_samples, data, sampleCount);
        m_frameCount = sampleCount;
        m_framesRead = 0;
        m_channels = channels;
        return true;
    }

    void Sound::Play()
    {
        m_framesRead = 0;
        AudioType::Play();
    }

    void Sound::Stop()
    {
        AudioType::Stop();
        m_framesRead = 0;
    }

    ui32 Sound::ReadFrames(float *output, ui32 frameCount)
    {
        const ui32 readFrames = Util::Clamp<ui32>(frameCount * m_channels, 0, m_frameCount - m_framesRead);
        for(ui32 i = 0; i < readFrames; ++i)
            output[i] += m_samples[m_framesRead + i] * m_volume;

        m_framesRead += readFrames;
        return frameCount;
    }

}