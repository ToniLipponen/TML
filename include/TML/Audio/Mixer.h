#pragma once
#include <TML/Audio/AudioEffect.h>
#include <TML/Export.h>
#include <cstdint>
#include <string>

namespace tml::Mixer
{
    TML_API void SetGain(float gain) noexcept;
    TML_API AudioEffect* AddEffect(const std::string& name, AudioEffect* effect);
    TML_API AudioEffect* GetEffect(const std::string& name);
    TML_API void RemoveEffect(const std::string& name);
}
