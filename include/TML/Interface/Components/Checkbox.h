#pragma once
#include "../BaseComponent.h"

namespace tml::Interface
{
    class TML_API Checkbox : public BaseComponent
    {
    public:
        Checkbox(int32_t x, int32_t y, uint32_t size, bool checked = false);
        bool GetValue() const { return m_value; }
    private:
        void pDraw(RenderTarget& renderer) override;
        bool m_value = false;
    };
}