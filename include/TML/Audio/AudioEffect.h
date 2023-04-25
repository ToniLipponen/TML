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
        
        void Enable()
        {
            m_enabled = true;
        }

        void Disable()
        {
            m_enabled = false;
        }

        [[nodiscard]] bool IsEnabled() const
        {
            return m_enabled;
        }
    private:
        bool m_enabled = true;
    };
}