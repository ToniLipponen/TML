#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Interface/Layout.h>

namespace tml
{
    namespace Interface
    {
        class HorizontalLayout : public BaseComponent, public Layout
        {
        public:
            HorizontalLayout();
            HorizontalLayout(i32 x, i32 y, ui32 w, ui32 h);

        private:
            void Draw(RenderWindow& renderWindow) override {}
            void ScaleChildren();
            void AlignChildren();
        };
    }
}