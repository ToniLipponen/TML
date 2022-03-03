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
            explicit Button(const std::string& text, i32 x = 0, i32 y = 0, ui32 w = 0, ui32 h = 0,
                            const UIFunc& onClick = nullptr, bool expand = false);
            void SetText(const std::string& str);
            void SetTextSize(float s) { m_text.SetSize(s);}
            inline constexpr const String& GetText() const noexcept { return m_text.GetString(); }
            void SetTextColor(const Color& color){ m_text.SetColor(color); }
        private:
            Text m_text;
            void pDraw(Renderer &renderer) override;
        };
    }
}