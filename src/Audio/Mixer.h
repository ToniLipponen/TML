#pragma once
#include <TML/Types.h>
#include <TML/Audio/Music.h>
#include <map>
#include <memory>

namespace tml::Mixer
{
    bool Init();
    void SetGain(float gain);
    void AddSound(ui32 id, AudioType* snd);
    void RemoveSound(ui32 id);
}
