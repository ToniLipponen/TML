#include <TML/Interface/Components/Progressbar.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    namespace Interface
    {
        Progressbar::Progressbar(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(w,h);
        }

        void Progressbar::Draw()
        {
            m_text.SetString(std::to_string(ui32(m_value * 100)) + "%");
            m_text.SetSize(m_size.y / 2);
            m_text.SetPosition({m_pos.x + (m_size.x / 2) - (m_text.GetDimensions().x / 2), static_cast<float>(m_pos.y + m_size.y / 4)});

            Renderer::DrawRect(m_pos, m_size, m_pColor);
            Renderer::DrawRect(m_pos, {m_size.x * m_value, static_cast<float>(m_size.y)}, m_activeColor);
            Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor);
            Renderer::Draw(m_text);
        }
    }
}