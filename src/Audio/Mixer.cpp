#include <TML/Audio/Mixer.h>
#include <TML/Audio/AudioEffect.h>
#include <TML/Audio/AudioType.h>
#include <TML/TMLAssert.h>

#define STB_VORBIS_HEADER_ONLY
#include <miniaudio/decoders/stb_vorbis.c>  /* Enables Vorbis decoding. */

#define DR_FLAC_IMPLEMENTATION
#include <miniaudio/decoders/dr_flac.h>     /* Enables FLAC decoding. */

#define DR_MP3_IMPLEMENTATION
#include <miniaudio/decoders/dr_mp3.h>      /* Enables MP3 decoding. */

#define DR_WAV_IMPLEMENTATION
#include <miniaudio/decoders/dr_wav.h>      /* Enables WAV decoding. */

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

#undef STB_VORBIS_HEADER_ONLY
#include <miniaudio/decoders/stb_vorbis.c>

#include <map>
#include <memory>

namespace tml::Mixer
{
    static ma_device* s_outputDevice = nullptr;
    static std::map<uint64_t, AudioType*> s_sounds;
    static std::map<std::string, std::unique_ptr<AudioEffect>> s_effects;
    static uint64_t s_soundCount = 0;

    void OnAudioCallback(void* device, void* output, const void*, ma_uint32 frameCount)
    {
        auto* outputBuffer = static_cast<tml::AudioFrame*>(output);

        if(!s_sounds.empty())
        {
            for(auto& [id, sound] : s_sounds)
            {
                if(sound->IsPlaying())
                {
                    const uint32_t frames = sound->ReadFrames(outputBuffer, frameCount);

                    if(frames == 0)
                    {
                        sound->Stop();
                        
                        if(sound->IsLooping())
                        {
                            sound->Play();
                        }
                    }
                }
            }

            for(auto& [name, effect] : s_effects)
            {
                if(effect->IsEnabled())
                {
                    for(uint32_t i = 0; i < frameCount; i++)
                    {
                        effect->Process(outputBuffer + i, 0, 0);
                    }
                }
            }

            for(uint32_t i = 0; i < frameCount; i++)
            {
                outputBuffer[i] *= s_outputDevice->masterVolumeFactor;
            }
        }
    }

    static void InitializeMixer()
    {
        ma_device_config config     = ma_device_config_init(ma_device_type_playback);
        config.playback.format      = ma_format_f32;
        config.playback.channels    = 2;
        config.sampleRate           = 48000;
        config.dataCallback         = (ma_device_callback_proc)OnAudioCallback;
        config.stopCallback         = nullptr;

        s_outputDevice = new ::ma_device;
        auto result = ma_device_init(nullptr, &config, s_outputDevice);
        s_outputDevice->masterVolumeFactor = 1;

        if(result == MA_SUCCESS)
        {
            ma_device_start(s_outputDevice);
        }
        else
        {
            std::puts("[Error]: Failed to initialize audio output device");
        }
    }

    void SetGain(float gain) noexcept
    {
        s_outputDevice->masterVolumeFactor = gain;
    }

    void AddSound(uint64_t id, AudioType* sound) noexcept
    {
        if(s_outputDevice == nullptr)
        {
            InitializeMixer();
        }

        s_sounds.insert(std::pair<uint32_t, AudioType*>(id, sound));
    }

    void RemoveSound(uint64_t id) noexcept
    {
        if(s_sounds.find(id) != s_sounds.end())
        {
            s_sounds.erase(id);
        }
    }

    uint64_t GetAudioID() noexcept
    {
        return s_soundCount++;
    }

    AudioEffect* AddEffect(const std::string& name, AudioEffect* effect)
    {
        s_effects[name] = std::unique_ptr<AudioEffect>(effect);
        return effect;
    }

    AudioEffect* GetEffect(const std::string& name)
    {
        if(s_effects.find(name) == s_effects.end())
        {
            return nullptr;
        }

        return s_effects.at(name).get();
    }

    void RemoveEffect(const std::string& name)
    {
        s_effects.erase(name);
    }
}

// namespace tml
// {
//     void MixerOnAudioCallback(void* device, void* pOutput, const void* pInput, ma_uint32 frameCount)
//     {
//         (void)pInput;
//         auto* maDevice = static_cast<ma_device*>(device);
//         auto* mixer = static_cast<Mixer*>(maDevice->pUserData);
//         auto& sounds = mixer->m_sounds;
//         auto* pOutputF32 = static_cast<float*>(pOutput);

//         if(!sounds.empty())
//         {
//             for(auto& [id, sound] : sounds)
//             {
//                 if(sound->IsPlaying())
//                 {
//                     const uint32_t frames = sound->ReadFrames(pOutputF32, frameCount);

//                     if(frames < frameCount)
//                     {
//                         sound->Stop();
//                         if(sound->IsLooping())
//                         {
//                             sound->Play();
//                         }
//                     }
//                 }
//             }

//             for(uint32_t i = 0; i < frameCount*2; i += 2)
//             {
//                 for(auto& [name, effect] : mixer->m_effects)
//                 {
//                     effect->Process(pOutputF32 + i, 0, 0);
//                 }
//             }

//             for(uint32_t i = 0; i < frameCount*2; ++i)
//             {
//                 pOutputF32[i] *= maDevice->masterVolumeFactor;
//             }
//         }
//     }

//     Mixer::Mixer()
//     {
//         ma_device_config config     = ma_device_config_init(ma_device_type_playback);
//         config.playback.format      = ma_format_f32;
//         config.playback.channels    = 2;
//         config.sampleRate           = 48000;
//         config.dataCallback         = (ma_device_callback_proc)MixerOnAudioCallback;
//         config.stopCallback         = nullptr;
//         config.pUserData            = this;

//         m_outputDevice = new ::ma_device;
//         auto* outputDevice = static_cast<::ma_device*>(m_outputDevice);
//         auto result = ma_device_init(nullptr, &config, outputDevice);
//         outputDevice->masterVolumeFactor = 1;

//         if(result == MA_SUCCESS)
//         {
//             ma_device_start(outputDevice);
//         }
//         else
//         {
//             std::puts("[Error]: Failed to initialize audio output device");
//         }
//     }

//     Mixer::~Mixer()
//     {
//         auto* outputDevice = static_cast<ma_device*>(m_outputDevice);
//         ma_device_stop(outputDevice);
//         ma_device_uninit(outputDevice);
//         delete outputDevice;
//     }

//     Mixer& Mixer::GetInstance() noexcept
//     {
//         static Mixer instance;
//         return instance;
//     }

//     void Mixer::SetGain(float gain) noexcept
//     {
//         reinterpret_cast<ma_device*>(m_outputDevice)->masterVolumeFactor = gain;
//     }

//     void Mixer::AddSound(uint64_t id, AudioType* sound) noexcept
//     {
//         m_sounds.insert(std::pair<uint32_t, AudioType*>(id, sound));
//     }

//     void Mixer::RemoveSound(uint64_t id) noexcept
//     {
//         if(m_sounds.find(id) != m_sounds.end())
//         {
//             m_sounds.erase(id);
//         }
//     }

//     uint64_t Mixer::GetAudioID() noexcept
//     {
//         return m_soundCount++;
//     }

//     AudioEffect* Mixer::AddEffect(const std::string& name, AudioEffect* effect)
//     {
//         m_effects[name] = std::unique_ptr<AudioEffect>(effect);
//         return effect;
//     }

//     AudioEffect* Mixer::GetEffect(const std::string& name)
//     {
//         if(m_effects.find(name) == m_effects.end())
//         {
//             return nullptr;
//         }

//         return m_effects.at(name).get();
//     }

//     void Mixer::RemoveEffect(const std::string& name)
//     {
//         m_effects.erase(name);
//     }
// }
