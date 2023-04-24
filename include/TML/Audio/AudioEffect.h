#pragma once
#include <TML/Audio/Common.h>
#include <cstdint>

namespace tml
{
    struct AudioEffect
    {
        AudioEffect(){}
        virtual ~AudioEffect(){}
        virtual void Process(tml::AudioFrame* frame, uint32_t frameN, uint32_t maxFrames) = 0;
    };
}