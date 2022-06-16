#pragma once
#include "../BaseComponent.h"
#include <TML/Graphics/Drawable/Text.h>

namespace tml::Interface
{
    class TML_API TextInput : public BaseComponent
    {
    public:
        TextInput(uint32_t width, uint32_t height = 20, int32_t x = 0, int32_t y = 0) noexcept;
        void SetValue(const String& string) noexcept;
        void SetTextColor(const Color& color) noexcept override;
        const String& GetValue() const noexcept;
        void SetReadOnly(bool readOnly) noexcept;

    protected:
        void AlignText() noexcept;

    private:
        void InitListeners() noexcept;
        void pDraw(RenderTarget& target) noexcept override;
        Text m_text;
        String m_value;
        int32_t m_cursorIndex = 0;
        double m_blinkTimer = 0;
        float m_cursorPos = 0;
        float m_roundness = 100;
        bool m_showLine = true;
        bool m_readOnly = false;
        double m_borderAnimationProgress = 0;
        Color m_borderColor;
    };
}