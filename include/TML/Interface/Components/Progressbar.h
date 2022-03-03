#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml
{
    namespace Interface
    {
        class Progressbar : public BaseComponent
        {
        public:
            Progressbar(i32 x, i32 y, ui32 w, ui32 h = 30);
            float GetValue() const noexcept;
            void SetValue(float value) noexcept;
            void SetTextColor(const Color& color) noexcept;
        private:
            void pDraw(Renderer &renderer) override;
            float m_value = 0;
            Text m_text;
        };
    }
}