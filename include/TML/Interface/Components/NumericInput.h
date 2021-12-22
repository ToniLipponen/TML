#pragma once
#include <TML/Interface/BaseComponent.h>
#include "Button.h"

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
            void OnUpdate(double dt) override;
            void OnMoved() override;
            void OnResized() override;
            virtual void Draw() override;
            T m_value = 0;
            T m_increment = 1;
            Button* m_incrementButton;
            Button* m_decrementButton;
            std::string m_valueStr;
            ui32 m_cursorIndex = 0;
            float m_repeatTimer;
        };
    }
}