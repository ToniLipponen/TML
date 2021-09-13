#include "Mixer.h"

#define DR_FLAC_IMPLEMENTATION
#include <miniaudio/decoders/dr_flac.h>  /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include <miniaudio/decoders/dr_mp3.h>   /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include <miniaudio/decoders/dr_wav.h>   /* Enables WAV decoding. */

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

ma_device OUTPUT_DEVICE;

ma_event s_stopEvent; /* <-- Signaled by the audio thread, waited on by the main thread. */

static float s_gain = 1.f;
std::map<tml::ui32, tml::Sound*> s_sounds;
ma_uint32 read_and_mix_pcm_frames_f32(ma_decoder* pDecoder, float* pOutputF32, ma_uint32 frameCount)
{
    float temp[4096];
    ma_uint32 tempCapInFrames = ma_countof(temp) / pDecoder->outputChannels; // Todo
    ma_uint32 totalFramesRead = 0;

    while (totalFramesRead < frameCount) {
        ma_uint32 iSample;
        ma_uint32 framesReadThisIteration;
        ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
        ma_uint32 framesToReadThisIteration = tempCapInFrames;
        if (framesToReadThisIteration > totalFramesRemaining) {
            framesToReadThisIteration = totalFramesRemaining;
        }

        framesReadThisIteration = (ma_uint32)ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration);
        if (framesReadThisIteration == 0) {
            break;
        }

        for (iSample = 0; iSample < framesReadThisIteration*pDecoder->outputChannels; ++iSample) {
            pOutputF32[totalFramesRead*pDecoder->outputChannels + iSample] += temp[iSample];
        }

        totalFramesRead += framesReadThisIteration;

        if (framesReadThisIteration < framesToReadThisIteration) {
            break;
        }
    }

    return totalFramesRead;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    float* pOutputF32 = (float*)pOutput;
    if(!s_sounds.empty())
    {
        for(auto sound : s_sounds)
        {
            if(sound.second->IsPlaying())
            {
                ma_uint32 frames = read_and_mix_pcm_frames_f32((ma_decoder*)sound.second->GetDecoder(), pOutputF32, frameCount);
                if(frames < frameCount)
                {
                    sound.second->Stop();
                    s_sounds.erase(sound.first);
                }
            }
        }
    }
}

namespace tml
{
    namespace Mixer
    {
        bool Init()
        {
            ma_device_config config     = ma_device_config_init(ma_device_type_playback);
            config.playback.format      = ma_format_f32;
            config.playback.channels    = 2;
            config.sampleRate           = 44100;
            config.dataCallback         = data_callback;
            config.stopCallback         = nullptr;
            config.pUserData            = nullptr;
            auto result = ma_device_init(NULL, &config, &OUTPUT_DEVICE);
            if(result == MA_SUCCESS)
            {
                ma_event_init(&s_stopEvent);
                ma_device_start(&OUTPUT_DEVICE);
                return true;
            }
            return false;
        }

        void SetGain(float gain)
        {
            s_gain = gain;
        }

        void AddSound(ui32 id, Sound* snd)
        {
            s_sounds.insert({id, snd});
            OUTPUT_DEVICE.pUserData = snd->GetDecoder();
        }

    }
}
