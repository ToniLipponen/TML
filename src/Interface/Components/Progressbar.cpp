#include <TML/Interface/Components/Progressbar.h>
#include <TML/Renderer.h>

namespace tml
{
    namespace Interface
    {
        Progressbar::Progressbar(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(w,h);
            m_sColor = 0x4d8be4ff;
        }

        void Progressbar::Draw()
        {
            Renderer::DrawRect(m_pos, m_size, m_pColor);
            Renderer::DrawRect(m_pos, {m_size.x * m_value, m_size.y}, m_sColor);
            Renderer::DrawGrid(m_pos+Vector2(1,1), m_size-Vector2(1,1), 1, 1, m_sColor);
        }
    }
}