#pragma once
#include <cstdint>
#include <TML/Audio/AudioType.h>

namespace tml::Mixer
{
    void AddSound(uint64_t id, AudioType* snd) noexcept;
    void RemoveSound(uint64_t id) noexcept;
    uint64_t GetAudioID() noexcept;
}