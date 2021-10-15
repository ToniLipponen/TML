#pragma once
#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    class FreeFormLayout : public Layout
    {
    public:
        using Layout::Layout;
    private:
        virtual void Draw(){}
        virtual void UpdateComponents() override {}
    };
}