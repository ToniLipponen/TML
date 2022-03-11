#include <TML/Audio/Mixer.h>
#include <_Assert.h>

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

namespace tml
{
    Mixer* Mixer::s_instance = nullptr;

    void MixerOnAudioCallback(void* device, void* pOutput, const void* pInput, ma_uint32 frameCount)
    {
        auto* maDevice = reinterpret_cast<ma_device*>(device);
        auto* mixer = reinterpret_cast<Mixer*>(maDevice->pUserData);
        auto& sounds = mixer->m_sounds;

        auto* pOutputF32 = static_cast<float*>(pOutput);
        if(!sounds.empty())
        {
            for(auto sound : sounds)
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
            for(ui32 i = 0; i < frameCount*2; ++i)
                pOutputF32[i] *= maDevice->masterVolumeFactor;
        }
    }

    Mixer::Mixer()
    {
        ma_device_config config     = ma_device_config_init(ma_device_type_playback);
        config.playback.format      = ma_format_f32;
        config.playback.channels    = 2;
        config.sampleRate           = 48000;
        config.dataCallback         = (ma_device_callback_proc)MixerOnAudioCallback;
        config.stopCallback         = nullptr;
        config.pUserData            = this;

        m_outputDevice = new ::ma_device;
        auto* outputDevice = static_cast<::ma_device*>(m_outputDevice);
        auto result = ma_device_init(nullptr, &config, outputDevice);
        outputDevice->masterVolumeFactor = 1;
        if(result == MA_SUCCESS)
            ma_device_start(outputDevice);
        else
            Logger::ErrorMessage("Failed to initialize audio output device");
    }

    Mixer& Mixer::GetInstance() noexcept
    {
        if(s_instance == nullptr)
            s_instance = new Mixer;

        return *s_instance;
    }

    void Mixer::SetGain(float gain) noexcept
    {
        reinterpret_cast<ma_device*>(m_outputDevice)->masterVolumeFactor = gain;
    }

    void Mixer::AddSound(ui64 id, AudioType* sound) noexcept
    {
        m_sounds.insert(std::pair<ui32, AudioType*>(id, sound));
    }

    void Mixer::RemoveSound(ui64 id) noexcept
    {
        if(m_sounds.find(id) != m_sounds.end())
            m_sounds.erase(id);
    }

    ui64 Mixer::GetAudioID() noexcept
    {
        return m_soundCount++;
    }
}
