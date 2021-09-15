#include <TML/Audio/Audio.h>
#include "Mixer.h"

namespace tml
{
    namespace Audio
    {
        void SetMasterVolume(float vol)
        {
            Mixer::SetGain(vol);
        }
    }
}