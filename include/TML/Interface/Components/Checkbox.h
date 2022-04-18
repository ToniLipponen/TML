#pragma once
#include "../BaseComponent.h"

namespace tml
{
    namespace Interface
    {
        class Checkbox : public BaseComponent
        {
        public:
            Checkbox(int32_t x, int32_t y, uint32_t size, bool checked = false);
            bool GetValue() const { return m_value; }
        private:
            void pDraw(Renderer &renderer) override;
            bool m_value = false;
        };
    }
}