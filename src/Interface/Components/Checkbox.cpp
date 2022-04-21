#include <TML/Interface/Components/Checkbox.h>

namespace tml
{
    namespace Interface
    {
        Checkbox::Checkbox(int32_t x, int32_t y, uint32_t size, bool checked)
        {
            m_pos = Vector2f(x,y);
            m_size = Vector2f(size,size);
            m_value = checked;
            AddListener("MouseDown", [&](BaseComponent*, Event& e){
                if(m_state.MouseOver)
                    m_state.MouseDown = e.mouseButton.button;
            });

            AddListener("Click", [&](BaseComponent*, Event& e)
            {
                m_value = !m_value;
            });
        }

        void Checkbox::pDraw(Renderer &window)
        {
            window.DrawRect(m_pos, m_size, m_pColor);
            window.DrawGrid(m_pos, m_size, 1, 1, m_sColor);
            if(m_value)
                window.DrawRect(static_cast<Vector2f>(m_pos) + static_cast<Vector2f>(m_size) * 0.2f, static_cast<Vector2f>(m_size) * 0.6f, m_activeColor);
        }
    }
}

