#pragma once
#include <TML/Interface/InterfaceComponent.h>

namespace tml::Interface
{
    class VerticalLayout : public BaseComponent
    {
    public:
        VerticalLayout(i32 x, i32 y, ui32 w, ui32 h);

    private:
        void Draw() override;
        virtual void OnUpdate(float dt) override;
    };
}