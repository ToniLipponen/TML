#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/Drawable/Text.h>

namespace tml::Interface
{
    class TML_API Label : public BaseComponent
    {
    public:
        explicit Label(const std::string& labelText, uint32_t s = 30, uint32_t width = 0, int32_t x = 0, int32_t y = 0) noexcept;
        void SetValue(std::string str) noexcept;
        bool ContainsPoint(const Vector2i& p) noexcept override;

    private:
        void pDraw(RenderTarget& renderer) noexcept override;
        Text m_text;
    };
}