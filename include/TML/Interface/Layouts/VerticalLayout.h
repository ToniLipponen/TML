#pragma once
#include <TML/Interface/InterfaceComponent.h>

namespace tml
{
    namespace Interface
    {
        class VerticalLayout : public BaseComponent
        {
        public:
            VerticalLayout(i32 x, i32 y, ui32 w, ui32 h);

        private:
            void Draw() override;
            void OnUpdate(double dt) override;
        };
    }
}