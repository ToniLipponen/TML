#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/Drawable/Text.h>

namespace tml::Interface
{
    class TML_API Label : public BaseComponent
    {
    public:
        Label(int32_t x, int32_t y, uint32_t s = 20, std::string str = "Label") noexcept;
        void SetValue(std::string str) noexcept;
        bool ContainsPoint(const Vector2i& p) noexcept override;

    private:
        void pDraw(RenderTarget& renderer) noexcept override;
        Text m_text;
    };
}