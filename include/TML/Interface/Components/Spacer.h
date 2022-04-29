#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml
{
    namespace Interface
    {
        class Spacer : public BaseComponent
        {
        public:
            Spacer();
        private:
            void pDraw(RenderTarget& renderTarget) override {}
        };
    }
}