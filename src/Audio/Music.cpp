#include <TML/Audio/Music.h>
#include <miniaudio/miniaudio.h>
#include <TML/System/File.h>
#include <TML/Audio/Mixer.h>

namespace tml
{
    Music::Music(const std::string &filename)
    {
        LoadFromFile(filename);
    }

    Music::Music(const char* data, ui32 bytes)
    {
        LoadFromData(data, bytes);
    }

    Music::~Music()
    {
        auto* decoder = static_cast<ma_decoder*>(m_decoder);
        Mixer::GetInstance().RemoveSound(m_id);
        ma_decoder_uninit(decoder);
        delete decoder;
    }

    bool Music::LoadFromFile(const std::string &filename)
    {
        m_state = Stopped;
        Mixer::GetInstance().RemoveSound(m_id);
        if(!m_decoder)
            m_decoder = new ma_decoder;

        static ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);

        auto* decoder = reinterpret_cast<ma_decoder*>(m_decoder);
        ma_result result = ma_decoder_init_file(filename.c_str(), &config, decoder);
        m_valid = (result == MA_SUCCESS);

        if(!m_valid)
        {
            tml::Logger::ErrorMessage("Failed to load sound file -> %s", filename.c_str());
            return false;
        }
        m_frameCount = ma_decoder_get_length_in_pcm_frames(decoder) * decoder->outputChannels;
        m_rate = decoder->outputSampleRate;
        m_channels = decoder->outputChannels;
        return true;
    }

    bool Music::LoadFromData(const char* data, ui32 bytes)
    {
        m_state = Stopped;
        Mixer::GetInstance().RemoveSound(m_id);
        if(!m_decoder)
            m_decoder = new ma_decoder;

        static ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
        auto* decoder = reinterpret_cast<ma_decoder*>(m_decoder);
        ma_result result = ma_decoder_init_memory(data, bytes, &config, decoder);
        m_valid = (result == MA_SUCCESS);

        if(!m_valid)
        {
            tml::Logger::ErrorMessage("Failed to load sound from memory.");
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
            ma_decoder_seek_to_pcm_frame((ma_decoder*)m_decoder, 0);
        m_state = Stopped;
        AudioType::Stop();
    }

    ui32 Music::ReadFrames(float *output, ui32 frameCount)
    {
        auto* decoder = (ma_decoder*)m_decoder;
        float temp[4096];
        ui32 tempCapInFrames = (sizeof(temp) / sizeof(float)) / decoder->outputChannels;
        ui32 totalFramesRead = 0;

        while(totalFramesRead < frameCount)
        {
            ui32 iSample;
            ui32 framesReadThisIteration;
            ui32 totalFramesRemaining = frameCount - totalFramesRead;
            ui32 framesToReadThisIteration = tempCapInFrames;
            if(framesToReadThisIteration > totalFramesRemaining)
                framesToReadThisIteration = totalFramesRemaining;

            framesReadThisIteration = (ui32)ma_decoder_read_pcm_frames(decoder, temp, framesToReadThisIteration);
            if(framesReadThisIteration == 0)
                break;

            for(iSample = 0; iSample < framesReadThisIteration*decoder->outputChannels; ++iSample)
                output[totalFramesRead*decoder->outputChannels + iSample] += temp[iSample] * m_volume;

            totalFramesRead += framesReadThisIteration;

            if(framesReadThisIteration < framesToReadThisIteration)
                break;
        }
        m_framesRead += totalFramesRead * m_channels;
        return totalFramesRead;
    }
}