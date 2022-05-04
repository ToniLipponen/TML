#pragma once
#include <cstdint>
#include "Music.h"
#include <map>
#include <memory>
#include <TML/Export.h>

namespace tml
{
    class TML_API Mixer
    {
    private:
        Mixer();
        ~Mixer();

    public:
        static Mixer& GetInstance() noexcept;
        void SetGain(float gain) noexcept;
        void AddSound(uint64_t id, AudioType* snd) noexcept;
        void RemoveSound(uint64_t id) noexcept;
        uint64_t GetAudioID() noexcept;

    private:
        friend void MixerOnAudioCallback(void* device, void* pOutput, const void* pInput, uint32_t frameCount);

    protected:
        void* m_outputDevice = nullptr;
        std::map<uint64_t, AudioType*> m_sounds;
        uint64_t m_soundCount = 0;
    };
}
