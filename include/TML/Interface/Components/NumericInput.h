#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml
{
    namespace Interface
    {
        template<typename T>
        class NumericInput : public BaseComponent
        {
        public:
            NumericInput(i32 x, i32 y, ui32 w, ui32 h, T value = 0);
            void SetValue(T value);
            T GetValue() const;

        private:
            void Increment();
            void Decrement();
            virtual void Draw(RenderWindow& renderWindow) override;
            T m_value = 0;
            T m_increment = 1;
            std::string m_valueStr;
            Text m_text;
            i32 m_cursorIndex = 0;
        };
    }
}