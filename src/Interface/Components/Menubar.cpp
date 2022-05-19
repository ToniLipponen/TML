#include <TML/Interface/Components/Menubar.h>

namespace tml::Interface
{
    Menubar::Menubar(RenderWindow &window)
    : HorizontalLayout(0,0, window.GetWidth(), 20)
    {
        m_vSizePolicy = Fixed;
        m_padding.x = 0;
    }

    void Menubar::pDraw(RenderTarget& target)
    {
        target.DrawRect(m_pos, m_size, m_pColor);
        target.DrawLine(m_pos + Vector2f(0, m_size.y), m_pos + m_size, 1, m_sColor, false);
    }
}