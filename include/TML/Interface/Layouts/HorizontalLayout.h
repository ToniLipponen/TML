#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    class TML_API HorizontalLayout : public BaseComponent, public Layout
    {
    public:
        HorizontalLayout() noexcept;
        HorizontalLayout(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept;

    private:
        void pDraw(RenderTarget& renderer) noexcept override {}
        void ScaleChildren() noexcept;
        void AlignChildren() noexcept;
    };
}