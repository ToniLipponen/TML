#pragma once
#include <TML/Utilities/Types.h>
#include <TML/Audio/Music.h>
#include <map>
#include <memory>

namespace tml
{
    namespace Mixer
    {
        bool Init();
        void SetGain(float gain);
        void AddSound(ui32 id, AudioType* snd);
        void RemoveSound(ui32 id);
    }
}
