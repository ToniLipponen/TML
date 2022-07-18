#pragma once
#include <TML/Interface/Component.h>

namespace tml::Interface
{
    class TML_API Spacer : public Component
    {
    public:
        Spacer() noexcept;

        Spacer(ComponentAxis axis, int size) noexcept;

    private:
        void pDraw(RenderTarget& renderer) noexcept override {}
    };
}