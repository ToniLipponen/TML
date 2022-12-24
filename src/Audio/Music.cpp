#include <TML/Audio/Music.h>
#include <TML/Audio/Mixer.h>
#include <miniaudio/miniaudio.h>
#include <cmath>
#include "TML/System/Math.h"

namespace tml
{
    Music::Music(const String &filename)
    {
        LoadFromFile(filename);
    }

    Music::Music(const char* data, uint32_t bytes)
    {
        LoadFromData(data, bytes);
    }

    Music::~Music()
    {
        auto* decoder = static_cast<ma_decoder*>(m_decoder);
        ma_decoder_uninit(decoder);
        delete decoder;
    }

    bool Music::LoadFromFile(const String &filename)
    {
        m_state = Stopped;
        Mixer::GetInstance().RemoveSound(m_id);

        if(!m_decoder)
        {
            m_decoder = new ma_decoder;
        }

        static ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);

        auto* decoder = reinterpret_cast<ma_decoder*>(m_decoder);
        ma_result result = ma_decoder_init_file(filename.c_str(), &config, decoder);
        m_valid = (result == MA_SUCCESS);

        if(!m_valid)
        {
            return false;
        }

        m_frameCount = ma_decoder_get_length_in_pcm_frames(decoder) * decoder->outputChannels;
        m_rate = decoder->outputSampleRate;
        m_channels = decoder->outputChannels;
        return true;
    }

    bool Music::LoadFromData(const char* data, uint32_t bytes)
    {
        m_state = Stopped;
        Mixer::GetInstance().RemoveSound(m_id);

        if(!m_decoder)
        {
            m_decoder = new ma_decoder;
        }

        static ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
        auto* decoder = reinterpret_cast<ma_decoder*>(m_decoder);
        ma_result result = ma_decoder_init_memory(data, bytes, &config, decoder);
        m_valid = (result == MA_SUCCESS);

        if(!m_valid)
        {
            return false;
        }

        m_frameCount = ma_decoder_get_length_in_pcm_frames(decoder) * decoder->outputChannels;
        m_rate = decoder->outputSampleRate;
        m_channels = decoder->outputChannels;
        return true;
    }

    void Music::Stop()
    {
        if(m_valid)
        {
            ma_decoder_seek_to_pcm_frame((ma_decoder*)m_decoder, 0);
        }

        m_state = Stopped;
        AudioType::Stop();
    }

    uint32_t Music::ReadFrames(float *output, uint32_t frameCount)
    {
        auto* decoder = (ma_decoder*)m_decoder;
        float temp[4096];
        uint32_t tempCapInFrames = (sizeof(temp) / sizeof(float)) / decoder->outputChannels;
        uint32_t totalFramesRead = 0;

        float left  = Math::Map<float>(m_balance, 1, 0, 0, 1);
        float right = Math::Map<float>(m_balance, -1, 0, 0, 1);

        while(totalFramesRead < frameCount)
        {
            uint32_t iSample;
            uint32_t framesReadThisIteration;
            uint32_t totalFramesRemaining = frameCount - totalFramesRead;
            uint32_t framesToReadThisIteration = tempCapInFrames;

            if(framesToReadThisIteration > totalFramesRemaining)
            {
                framesToReadThisIteration = totalFramesRemaining;
            }

            framesReadThisIteration = (uint32_t)ma_decoder_read_pcm_frames(decoder, temp, framesToReadThisIteration);

            if(framesReadThisIteration == 0)
            {
                break;
            }

            for(iSample = 0; iSample < framesReadThisIteration * decoder->outputChannels; iSample += 2)
            {
                output[totalFramesRead * decoder->outputChannels + iSample    ] += temp[iSample    ] * m_volume * left;
                output[totalFramesRead * decoder->outputChannels + iSample + 1] += temp[iSample + 1] * m_volume * right;
            }

            totalFramesRead += framesReadThisIteration;

            if(framesReadThisIteration < framesToReadThisIteration)
            {
                break;
            }
        }

        m_framesRead += totalFramesRead * m_channels;
        return totalFramesRead;
    }
}