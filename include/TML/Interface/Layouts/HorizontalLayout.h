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
            HorizontalLayout(i32 x, i32 y, ui32 w, ui32 h);

        private:
            void Draw() override {}
            void OnUpdate(double dt) override;
            void ScaleChildren();
            void AlignChildren();
        };
    }
}