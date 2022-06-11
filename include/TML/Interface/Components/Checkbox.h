#pragma once
#include "../BaseComponent.h"

namespace tml::Interface
{
    class TML_API Checkbox : public BaseComponent
    {
    public:
        Checkbox(int32_t x, int32_t y, uint32_t size, bool checked = false) noexcept;
        bool GetValue() const noexcept;

    private:
        void pDraw(RenderTarget& renderer) noexcept override;
        bool m_value = false;
    };
}