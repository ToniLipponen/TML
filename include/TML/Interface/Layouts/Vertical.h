#pragma once
#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    class VerticalLayout : public Layout
    {
    public:
        using Layout::Layout;
    private:
        virtual void UpdateComponents();
    };
}