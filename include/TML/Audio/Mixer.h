#pragma once
#include "TML/Types.h"
#include "Music.h"
#include <map>
#include <memory>

namespace tml
{
    class Mixer
    {
    private:
        Mixer();

    public:
        static Mixer& GetInstance() noexcept;
        void SetGain(float gain) noexcept;
        void AddSound(ui64 id, AudioType* snd) noexcept;
        void RemoveSound(ui64 id) noexcept;
        ui64 GetAudioID() noexcept;

    private:
        friend void MixerOnAudioCallback(void* device, void* pOutput, const void* pInput, ui32 frameCount);

    protected:
        static Mixer* s_instance;
        void* m_outputDevice = nullptr;
        std::map<ui64, AudioType*> m_sounds;
        ui64 m_soundCount = 0;
    };
}
