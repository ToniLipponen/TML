#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Interface/Layout.h>

namespace tml
{
    namespace Interface
    {
        class VerticalLayout : public BaseComponent, public Layout
        {
        public:
            VerticalLayout(i32 x, i32 y, ui32 w, ui32 h);

        private:
            void Draw() override {};
            void ScaleChildren();
            void AlignChildren();
        };
    }
}