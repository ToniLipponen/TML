#pragma once
#include "../BaseComponent.h"
#include <TML/Graphics/Drawable/Text.h>

namespace tml
{
    namespace Interface
    {
        class TextInput : public BaseComponent
        {
        public:
            TextInput(i32 x, i32 y, i32 width, i32 height = 30);
            void SetValue(const std::string& string);
            std::string GetValue() const { return m_text.GetString(); }
        private:
            void InitListeners();
            void Draw(RenderWindow& renderWindow);
            Text m_text;
            std::wstring m_value;
            i32 m_cursorIndex = 0;
            double m_blinkTimer = 0;
            bool m_showLine = true;
            float m_cursorPos = 0;
        };
    }
}