#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/Drawable/Text.h>

namespace tml
{
    namespace Interface
    {
        template<typename T>
        class NumericInput : public BaseComponent
        {
        public:
            NumericInput(int32_t x, int32_t y, uint32_t w, uint32_t h, T value = 0);
            void SetValue(T value);
            T GetValue() const;

        private:
            void Increment();
            void Decrement();
            void pDraw(RenderTarget& renderTarget) override;
            T m_value = 0;
            T m_increment = 1;
            std::string m_valueStr;
            Text m_text;
            int32_t m_cursorIndex = 0;
        };
    }
}