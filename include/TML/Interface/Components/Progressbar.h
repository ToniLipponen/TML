#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Utilities/Utilities.h>

namespace tml
{
    namespace Interface
    {
        class Progressbar : public BaseComponent
        {
        public:
            Progressbar(i32 x, i32 y, ui32 w, ui32 h = 30);
            inline constexpr float GetValue() const noexcept { return m_value; }
            inline constexpr void SetValue(float value) noexcept { m_value = Math::Clamp(value, 0.0f, 1.0f); }
        private:
            void Draw() override;
            float m_value = 0;
            Text m_text;
        };
    }
}