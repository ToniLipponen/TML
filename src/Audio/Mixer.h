#pragma once
#include <TML/Types.h>
#include <TML/Audio/Sound.h>
#include <map>
#include <memory>

namespace tml
{
    namespace Mixer
    {
        bool Init();
        void SetGain(float gain);
        void AddSound(ui32 id, Sound* snd);
    };
}
