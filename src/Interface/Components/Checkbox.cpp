#include <TML/Interface/Components/Checkbox.h>
#include <TML/Renderer.h>

namespace tml::Interface
{
    Checkbox::Checkbox(i32 x, i32 y, ui32 size, bool checked)
    {
        m_pos = Vector2(x,y);
        m_size = Vector2(size,size);
        m_value = checked;
    }

    void Checkbox::Draw()
    {
        Renderer::DrawRect(m_pos, m_size, m_sColor, 8.f);
        Renderer::DrawRect(m_pos + m_size * 0.05f, m_size * 0.9f, m_pColor, 6.f);
        if(m_value)
            Renderer::DrawRect(m_pos + m_size * 0.2f, m_size * 0.6f, m_activeColor, 4.f);
        Renderer::DrawText(m_label, m_pos + Vector2(m_size.x, 0),m_size.y, BLACK);
    }

    void Checkbox::OnMouseClick(const Vector2& mousePos)
    {
        m_value = !m_value;
    }
}

