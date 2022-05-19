#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    class HorizontalLayout : public BaseComponent, public Layout
    {
    public:
        HorizontalLayout();
        HorizontalLayout(int32_t x, int32_t y, uint32_t w, uint32_t h);

    private:
        void pDraw(RenderTarget& renderer) override {}
        void ScaleChildren();
        void AlignChildren();
    };
}