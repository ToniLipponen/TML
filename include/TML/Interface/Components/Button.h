#pragma once
#include "../BaseComponent.h"
#include <TML/Graphics/Drawable/Text.h>

namespace tml
{
    namespace Interface
    {
        class Button : public BaseComponent
        {
        public:
            Button(i32 x, i32 y, ui32 w, ui32 h, std::string text = "", UIFunc onClick = nullptr);
            void SetText(const std::string& str);
            void SetTextSize(float s) { m_text.SetSize(s);}
            std::string GetText() const { return m_text.GetString(); }
            void SetTextColor(const Color& color){ m_text.SetColor(color); }
        private:
            Text m_text;
            void Draw(RenderWindow& renderWindow) override;
        };
    }
}