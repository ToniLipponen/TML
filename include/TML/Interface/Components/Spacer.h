#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml::Interface
{
    class TML_API Spacer : public BaseComponent
    {
    public:
        Spacer() noexcept;

        Spacer(ComponentAxis axis, int size) noexcept;

    private:
        void pDraw(RenderTarget& renderer) noexcept override {}
    };
}