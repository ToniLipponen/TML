#include <TML/Interface/Components/Checkbox.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    namespace Interface
    {
        Checkbox::Checkbox(i32 x, i32 y, ui32 size, bool checked)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(size,size);
            m_value = checked;
        }

        void Checkbox::Draw()
        {
            Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor);
            if(m_value)
                Renderer::DrawRect(m_pos + m_size * 0.2f, m_size * 0.6f, m_activeColor);
        }

        void Checkbox::OnMouseClick(const Vector2& mousePos)
        {
            m_value = !m_value;
        }
    }
}

