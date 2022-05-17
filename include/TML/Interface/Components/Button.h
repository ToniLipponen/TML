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
            explicit Button(const std::string& text, int32_t x = 0, int32_t y = 0, uint32_t w = 0, uint32_t h = 0,
                            const UIFunc& onClick = nullptr, bool expand = false);
            void SetRoundness(float roundness);
            void SetText(const std::string& str);
            void SetTextSize(float s) { m_text.SetSize(s);}
            void SetTextColor(const Color& color){ m_text.SetColor(color); }
            inline const String& GetText() const noexcept { return m_text.GetString(); }
        private:
            void pDraw(RenderTarget& renderTarget) override;
            Text m_text;
            float m_roundness = 0;
        };
    }
}