#include <TML/Interface/Components/Viewport.h>

namespace tml
{
    namespace Interface
    {
        Viewport::Viewport()
        {
            m_pColor = 0xc7c7c7ff;
            m_hSizePolicy = m_vSizePolicy = Expand;
            SetCallbacks();
        }

        Viewport::Viewport(int32_t x, int32_t y, uint32_t w, uint32_t h)
        : BaseComponent(x,y,w,h)
        {
            m_pColor = 0xc7c7c7ff;
            m_hSizePolicy = m_vSizePolicy = Expand;
            SetCallbacks();
        }

        void Viewport::pDraw(Renderer &window)
        {
            window.DrawTexture(m_surface.GetTexture(), m_pos, m_size);
            window.DrawGrid(m_pos, m_size, 1,1, m_pColor);
//            m_surface.
        }

        void Viewport::SetCallbacks()
        {
            m_surface.Create(m_size.x, m_size.y);
            m_surface.SetClearColor(Color::Black);
            // No callbacks
        }
    }
}