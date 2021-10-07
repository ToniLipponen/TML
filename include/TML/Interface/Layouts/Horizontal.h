#pragma once
#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    class HorizontalLayout : public Layout
    {
    public:
        using Layout::Layout;
    private:
        virtual void UpdateComponents();
        virtual void Draw();
    };
}