#include <TML/Audio/Music.h>
#include <TML/Audio/Mixer.h>
#include <TML/System/Math.h>
#include <miniaudio/miniaudio.h>
#include <cmath>
#include <array>
#include "MixerPriv.h"

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
        Mixer::RemoveSound(m_id);

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
        Mixer::RemoveSound(m_id);

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

    uint32_t Music::ReadFrames(AudioFrame* output, uint32_t frameCount)
    {
        auto* decoder = static_cast<ma_decoder*>(m_decoder);
        std::array<AudioFrame, 2048> temp;
        const uint32_t tempCapInFrames = temp.size();
        uint32_t totalFramesRead = 0;

        AudioFrame balance = {
            Math::Map<float>(m_balance, 1, 0, 0, 1),
            Math::Map<float>(m_balance, -1, 0, 0, 1)
        };

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

            framesReadThisIteration = (uint32_t)ma_decoder_read_pcm_frames(decoder, temp.data(), framesToReadThisIteration);

            if(framesReadThisIteration == 0)
            {
                break;
            }

            for(auto& [name, effect] : m_effects)
            {
                if(effect->IsEnabled())
                {
                    for(int i = 0; i < framesReadThisIteration; i++)
                    {
                        effect->Process(temp.data() + i, m_framesRead + totalFramesRead + i, m_frameCount);
                    }
                }
            }

            for(iSample = 0; iSample < framesReadThisIteration; iSample++)
            {
                output[totalFramesRead + iSample] += temp[iSample] * m_volume * balance;
            }

            totalFramesRead += framesReadThisIteration;

            if(framesReadThisIteration < framesToReadThisIteration)
            {
                break;
            }
        }

        m_framesRead += totalFramesRead;
        return totalFramesRead;
    }
}