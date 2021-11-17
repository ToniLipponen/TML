#pragma once
#include "../BaseComponent.h"

namespace tml
{
    namespace Interface
    {
        class Button : public BaseComponent
        {
        public:
            Button(ui32 x, ui32 y, ui32 w, ui32 h, std::string text = "", Function_ptr<void, BaseComponent*> onClick = nullptr);
            void SetText(const std::string& str);
            void SetTextSize(float s) { m_textSize = s; }
            std::string GetText() const { return m_text; }
            void SetTextColor(const Color& color){ m_tColor = color; }
        private:
            std::string m_text;
            float m_textSize = 15.f;
            Color m_tColor = BLACK;
            void Draw();
        };
    }
}