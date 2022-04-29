#include <TML/Interface/Components/Menubar.h>

namespace tml
{
    namespace Interface
    {
        Menubar::Menubar(RenderWindow &window)
        : HorizontalLayout(0,0, window.GetWidth(), 20)
        {
            m_vSizePolicy = Fixed;
            m_padding.x = 0;
        }

        void Menubar::pDraw(RenderTarget& renderTarget)
        {
            renderTarget.DrawRect(m_pos, m_size, m_pColor);
            renderTarget.DrawLine(m_pos + Vector2f(0, m_size.y), m_pos + m_size, 1, m_sColor, false);
        }
    }
}