#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    void Layout::SetPadding(const Vector2i &padding) noexcept
    {
        m_padding = padding;
    }

    Vector2i Layout::GetPadding() const noexcept
    {
        return m_padding;
    }
}