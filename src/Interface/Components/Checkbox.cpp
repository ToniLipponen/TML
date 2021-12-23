#include <TML/Interface/Components/Checkbox.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    namespace Interface
    {
        Checkbox::Checkbox(i32 x, i32 y, ui32 size, bool checked)
        {
            m_pos = Vector2f(x,y);
            m_size = Vector2f(size,size);
            m_value = checked;
            AddListener("Click", [&](BaseComponent*, Event& e)
            {
                if(m_state.MouseOver)
                    m_value = !m_value;
            });
        }

        void Checkbox::Draw()
        {
            Renderer::DrawRect(m_pos, m_size, m_pColor);
            Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor);
            if(m_value)
                Renderer::DrawRect(static_cast<Vector2f>(m_pos) + static_cast<Vector2f>(m_size) * 0.2f, static_cast<Vector2f>(m_size) * 0.6f, m_activeColor);
        }
    }
}

