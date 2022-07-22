#pragma once
#include "../Component.h"
#include <TML/Graphics/Drawable/Text.h>

namespace tml::Interface
{
    class TML_API LineInput : public Component
    {
    public:
        explicit LineInput(uint32_t width = 0, uint32_t height = 24, int32_t x = 0, int32_t y = 0) noexcept;
        void SetValue(const String& string) noexcept;
        void SetTextColor(const Color& color) noexcept override;
        void SetReadOnly(bool readOnly) noexcept;
        void SetPasswordField(bool passwordField) noexcept;
        const String& GetValue() const noexcept;
        bool GetReadOnly() const noexcept;
        bool GetPasswordField() const noexcept;

    protected:
        void AlignText() noexcept;
        void UpdateCursorPosition() noexcept;
        void UpdatePasswordText() noexcept;

    private:
        void InitListeners() noexcept;
        void pDraw(RenderTarget& target) noexcept override;
        Text m_text;
        Text m_passwordText; //!< Asterisks
        String m_value;
        int32_t m_cursorIndex = 0;
        double m_blinkTimer = 0;
        float m_cursorPos = 0;
        bool m_showLine = true;
        bool m_readOnly = false;
        bool m_password = false;
        double m_borderAnimationProgress = 0;
        Color m_borderColor;
    };
}