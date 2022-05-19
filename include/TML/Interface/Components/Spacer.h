#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml::Interface
{
    class TML_API Spacer : public BaseComponent
    {
    public:
        Spacer();
    private:
        void pDraw(RenderTarget& renderer) override {}
    };
}