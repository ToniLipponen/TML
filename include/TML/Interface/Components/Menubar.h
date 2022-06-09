#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Interface/Layouts/HorizontalLayout.h>

namespace tml::Interface
{
    class TML_API Menubar : public HorizontalLayout
    {
    public:
        explicit Menubar(RenderWindow& window) noexcept;
    private:
        void pDraw(RenderTarget& renderer) noexcept override;
    };
}