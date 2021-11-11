#include <TML/Interface/Components/Progressbar.h>
#include <TML/Renderer.h>

extern tml::Text* DEFAULT_TEXT;
namespace tml
{
    namespace Interface
    {
        Progressbar::Progressbar(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(w,h);
        }

        void Progressbar::Draw()
        {
            DEFAULT_TEXT->SetString(std::to_string(int(m_value * 100)) + "%");
            DEFAULT_TEXT->SetSize(m_size.y / 2);
            DEFAULT_TEXT->SetPosition({m_pos.x + (m_size.x / 2) - (DEFAULT_TEXT->GetDimensions().x / 2), m_pos.y + m_size.y / 4});

            Renderer::DrawRect(m_pos, m_size, m_pColor);
            Renderer::DrawRect(m_pos, {m_size.x * m_value, m_size.y}, m_activeColor);
            Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor);
            Renderer::Draw(*DEFAULT_TEXT);
        }
    }
}