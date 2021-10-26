#pragma once
#include <TML/Interface/InterfaceComponent.h>

namespace tml::Interface
{
    class HorizontalLayout : public BaseComponent
    {
    public:
        HorizontalLayout(i32 x, i32 y, ui32 w, ui32 h);

    private:
        void Draw() override {}
        virtual void OnUpdate(float dt) override;
    };
}