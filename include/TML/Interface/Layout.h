#pragma once
#include <TML/Types.h>

namespace tml
{
    namespace Interface
    {
        class Layout
        {
        public:
            enum AlignPolicy { Near, Far, Center };
        public:
            void SetHorizontalAlign(AlignPolicy alignPolicy) { m_hAlignPolicy = alignPolicy;}
            void SetVerticalAlign(AlignPolicy alignPolicy) { m_vAlignPolicy = alignPolicy;}
            inline constexpr AlignPolicy GetHorizontalAlign() const noexcept { return m_hAlignPolicy; }
            inline constexpr AlignPolicy GetVerticalAlign() const noexcept { return m_vAlignPolicy; }
            void SetPadding(const Vector2& padding){ m_padding = padding; }
            Vector2 GetPadding() const noexcept { return m_padding; }
        protected:
            AlignPolicy m_hAlignPolicy = Near; // Horizontal align policy.
            AlignPolicy m_vAlignPolicy = Near; // Vertical align policy.
            Vector2 m_padding = {5, 5};
        };
    }
}