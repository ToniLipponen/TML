#include "Mixer.h"
#include "../internal/Assert.h"

#define DR_FLAC_IMPLEMENTATION
#include <miniaudio/decoders/dr_flac.h>  /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include <miniaudio/decoders/dr_mp3.h>   /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include <miniaudio/decoders/dr_wav.h>   /* Enables WAV decoding. */

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

static float s_gain = 1.f;
static std::map<tml::ui32, tml::AudioType*> s_sounds;
static ma_device OUTPUT_DEVICE;

namespace tml
{
    ma_decoder_config s_decoder_config;
    void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
    {
        auto* pOutputF32 = (float*)pOutput;
        if(!s_sounds.empty())
        {
            for(auto sound : s_sounds)
            {
                if(sound.second->IsPlaying())
                {
                    const tml::ui32 frames = sound.second->ReadFrames(pOutputF32, frameCount);
                    if(frames < frameCount)
                    {
                        sound.second->Stop();
                        if(sound.second->IsLooping())
                            sound.second->Play();
                    }
                }
            }
            for(tml::ui32 i = 0; i < frameCount; i++)
                pOutputF32[i] *= s_gain;
        }
    }

    namespace Mixer
    {
        bool Init()
        {
            ma_device_config config     = ma_device_config_init(ma_device_type_playback);
            config.playback.format      = ma_format_f32;
            config.playback.channels    = 2;
            config.sampleRate           = 48000;
            config.dataCallback         = (ma_device_callback_proc)data_callback;
            config.stopCallback         = nullptr;
            config.pUserData            = nullptr;

            s_decoder_config.format = ma_format_unknown;
            s_decoder_config.channels = 2;
            s_decoder_config.sampleRate = 48000;

            auto result = ma_device_init(nullptr, &config, &OUTPUT_DEVICE);
            if(result == MA_SUCCESS)
            {
                ma_device_start(&OUTPUT_DEVICE);
                return true;
            }
            Logger::ErrorMessage("Failed to initialize audio output device");
            return false;
        }

        void SetGain(float gain)
        {
            s_gain = gain;
        }

        void AddSound(ui32 id, AudioType* snd)
        {
            s_sounds.insert({id, snd});
        }

        void RemoveSound(ui32 id)
        {
            s_sounds.erase(id);
        }
    }
}
