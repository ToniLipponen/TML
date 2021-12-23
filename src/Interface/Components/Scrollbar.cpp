#include <TML/Interface/Components/Scrollbar.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    namespace Interface
    {
        template<ComponentAxis axis>
        Scrollbar<axis>::Scrollbar(i32 x, i32 y, ui32 length, ui32 thickness)
        {
            m_pos = Vector2i(x,y);
            if(axis == Horizontal)
                m_size = Vector2i(length, thickness);
            else
                m_size = Vector2i(thickness, length);
            m_sColor = 0x4d8be4ff;
            m_pColor = 0xccccccff;

            AddListener("MouseDown", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseOver)
                {
                    m_state.MouseDown = e.mouseButton.button;
                    if(axis == Horizontal)
                        m_value = Math::Clamp<float>(float(e.mouseButton.x - m_pos.x) / float(m_size.x) * m_max, m_min, m_max);
                    else
                        m_value = m_max - Math::Clamp<float>(m_max - float((e.mouseButton.y - m_pos.y) / float(m_size.y) * m_max), m_min, m_max);
                }
            });
            AddListener("MouseMoved", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseDown != -1)
                {
                    if(axis == Horizontal)
                        m_value = Math::Clamp<float>(float(e.mouseMove.x - m_pos.x) / float(m_size.x) * m_max, m_min, m_max);
                    else
                        m_value = m_max - Math::Clamp<float>(m_max - float((e.mouseMove.y - m_pos.y) / float(m_size.y) * m_max), m_min, m_max);
                }
            });
        }

        template<ComponentAxis axis>
        void Scrollbar<axis>::SetValue(ui32 value)
        {
            m_value = Math::Clamp<i32>(value, m_min, m_max);
        }

        template<ComponentAxis axis>
        void Scrollbar<axis>::Draw()
        {
            if(axis == Vertical)
            {
                const float barSize = m_size.y / m_max;
                const auto barPos = Math::Min(m_pos.y + barSize * m_value, m_pos.y + m_size.y - barSize);

                Renderer::DrawRect(m_pos, m_size, m_pColor);
                Renderer::DrawRect(Vector2f(m_pos.x, barPos), Vector2f(m_size.x, barSize), m_sColor);
            }
            else
            {
                const float barSize = m_size.x / m_max;
                const auto barPos = Math::Min(m_pos.x + barSize * m_value, m_pos.x + m_size.x - barSize);

                Renderer::DrawRect(m_pos, m_size, m_pColor);
                Renderer::DrawRect(Vector2f(barPos, m_pos.y), Vector2f(barSize, m_size.y), m_sColor);
            }

        }


        template class Scrollbar<Horizontal>;
        template class Scrollbar<Vertical>;
    }
}