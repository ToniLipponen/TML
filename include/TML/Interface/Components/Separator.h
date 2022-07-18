#pragma once
#include <TML/Interface/Component.h>

namespace tml::Interface
{
    template<ComponentAxis axis>
    class TML_API Separator : public Component
    {
    public:
        explicit Separator(uint32_t thickness = 1) noexcept;

    private:
        void pDraw(RenderTarget& renderer) noexcept override;
        uint32_t m_thickness;
    };

    using VSeparator = Separator<ComponentAxis::Vertical>;
    using HSeparator = Separator<ComponentAxis::Horizontal>;
}