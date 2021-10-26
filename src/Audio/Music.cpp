#include <TML/Audio/Music.h>
#include <miniaudio/miniaudio.h>
#include "Mixer.h"
#include "../internal/Assert.h"

namespace tml
{
    extern ma_decoder_config s_decoder_config;
    volatile bool AudioInitialized = Mixer::Init();

    Music::Music()
    {
        m_decoder = new ma_decoder;
        m_id = s_soundCount++;
    }

    Music::Music(const std::string &filename)
    {
        m_decoder = new ma_decoder;
        ma_result result = ma_decoder_init_file(filename.c_str(), &s_decoder_config, (ma_decoder*)m_decoder);
        if (result != MA_SUCCESS)
            tml::Logger::ErrorMessage("Failed to load file -> %s", filename.c_str());
        m_id = s_soundCount++;
    }

    Music::Music(void *data, ui32 bytes)
    {
        m_decoder = new ma_decoder;
        ma_result result = ma_decoder_init_memory(data, bytes, &s_decoder_config, (ma_decoder*)m_decoder);
        if (result != MA_SUCCESS)
            tml::Logger::ErrorMessage("Failed to load music from memory.");
        m_id = s_soundCount++;
    }

    Music::~Music()
    {
        Mixer::RemoveSound(m_id);
        ma_decoder_uninit((ma_decoder*)m_decoder);
        delete (ma_decoder*)m_decoder;
    }

    bool Music::LoadFromFile(const std::string &filename)
    {
        m_state = Stopped;
        Mixer::RemoveSound(m_id);
        ma_decoder_uninit((ma_decoder*)m_decoder);
        ma_result result = ma_decoder_init_file(filename.c_str(), &s_decoder_config, (ma_decoder*)m_decoder);
        if (result != MA_SUCCESS)
        {
            tml::Logger::ErrorMessage("Failed to load sound file -> %s", filename.c_str());
            return false;
        }
        return true;
    }

    bool Music::LoadFromData(void *data, ui32 bytes)
    {
        m_state = Stopped;
        Mixer::RemoveSound(m_id);
        ma_decoder_uninit((ma_decoder*)m_decoder);

        ma_result result = ma_decoder_init_memory(data, bytes, &s_decoder_config, (ma_decoder*)m_decoder);
        if (result != MA_SUCCESS)
        {
            tml::Logger::ErrorMessage("Failed to load sound from memory.");
            return false;
        }
        return true;
    }

    void Music::Stop()
    {
        ma_decoder_seek_to_pcm_frame((ma_decoder*)m_decoder, 0);
        m_state = Stopped;
    }

    ui32 Music::ReadFrames(float *output, ui32 frameCount)
    {
        auto* decoder = (ma_decoder*)m_decoder;
        float temp[4096];
        ui32 tempCapInFrames = (sizeof(temp) / sizeof(float)) / decoder->outputChannels;
        ui32 totalFramesRead = 0;

        while (totalFramesRead < frameCount) {
            ui32 iSample;
            ui32 framesReadThisIteration;
            ui32 totalFramesRemaining = frameCount - totalFramesRead;
            ui32 framesToReadThisIteration = tempCapInFrames;
            if (framesToReadThisIteration > totalFramesRemaining) {
                framesToReadThisIteration = totalFramesRemaining;
            }

            framesReadThisIteration = (ui32)ma_decoder_read_pcm_frames(decoder, temp, framesToReadThisIteration);
            if (framesReadThisIteration == 0) {
                break;
            }

            for (iSample = 0; iSample < framesReadThisIteration*decoder->outputChannels; ++iSample) {
                output[totalFramesRead*decoder->outputChannels + iSample] += temp[iSample] * m_volume;
            }

            totalFramesRead += framesReadThisIteration;

            if (framesReadThisIteration < framesToReadThisIteration) {
                break;
            }
        }
        return totalFramesRead;
    }
};