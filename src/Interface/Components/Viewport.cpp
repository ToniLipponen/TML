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

        Viewport::Viewport(i32 x, i32 y, ui32 w, ui32 h)
        : BaseComponent(x,y,w,h)
        {
            m_pColor = 0xc7c7c7ff;
            m_hSizePolicy = m_vSizePolicy = Expand;
            SetCallbacks();
        }

        void Viewport::Draw(RenderWindow &window)
        {
            window.DrawTexture(m_surface.GetTexture(), m_pos, m_size);
            window.DrawGrid(m_pos, m_size, 1,1, m_pColor);
//            m_surface.
        }

        void Viewport::SetCallbacks()
        {
            m_surface.Create(m_size.x, m_size.y);
            m_surface.SetClearColor(Color::Black);
            AddListener("Resized", [&](BaseComponent* c, Event& e)
            {
//                m_surface.Create(e.size.x, e.size.y);
            });
        }
    }
}