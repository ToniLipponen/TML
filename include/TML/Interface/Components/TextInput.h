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
            TextInput(int32_t x, int32_t y, uint32_t width, uint32_t height = 30);
            void SetValue(const std::string& string);
            inline constexpr const String& GetValue() const noexcept { return m_text.GetString(); }
        private:
            void InitListeners();
            void pDraw(Renderer &renderer) override;
            Text m_text;
            String m_value;
            int32_t m_cursorIndex = 0;
            double m_blinkTimer = 0;
            bool m_showLine = true;
            float m_cursorPos = 0;
        };
    }
}