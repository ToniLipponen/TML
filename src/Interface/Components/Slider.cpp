#include <TML/Interface/Components/Slider.h>

namespace tml
{
    namespace Interface
    {
        template<ComponentAxis axis>
        Slider<axis>::Slider(i32 x, i32 y, ui32 size, ui32 thickness, float min, float max)
        {
            m_min = Math::Max<float>(min, 0);
            m_max = Math::Max<float>(max, 0);
            m_value = (m_min + m_max) / 2;

            if(axis == Horizontal)
            {
                m_pos = Vector2i(x,y);
                m_size = Vector2i(size, thickness);
                m_vSizePolicy = Fixed;
                m_hSizePolicy = Expand;
            }
            else
            {
                m_pos = Vector2i(x,y);
                m_size = Vector2i(thickness, size);
                m_vSizePolicy = Expand;
                m_hSizePolicy = Fixed;
            }

            AddListener("MouseDown", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseOver)
                {
                    m_state.MouseDown = e.mouseButton.button;
                    if(axis == Horizontal)
                        m_value = Math::Clamp(float(e.mouseButton.x - m_pos.x) / float(m_size.x) * m_max, m_min, m_max);
                    else
                        m_value = Math::Clamp(m_max - float((e.mouseButton.y - m_pos.y) / float(m_size.y) * m_max), m_min, m_max);
                }
            });

            AddListener("MouseMoved", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseDown != -1)
                {
                    if(axis == Horizontal)
                        m_value = Math::Clamp(float(e.mouseMove.x - m_pos.x) / float(m_size.x) * m_max, m_min, m_max);
                    else
                        m_value = Math::Clamp(m_max - float((e.mouseMove.y - m_pos.y) / float(m_size.y) * m_max), m_min, m_max);
                }
            });

        }

        template<ComponentAxis axis>
        void Slider<axis>::SetValue(float value)
        {
            m_value = Math::Clamp(value, m_min, m_max);
        }

        template<ComponentAxis axis>
        void Slider<axis>::pDraw(Renderer &window)
        {
            window.DrawRect(m_pos, m_size, m_pColor);
            if(m_state.Focused)
                window.DrawGrid(m_pos, m_size, 1, 1, m_activeColor);
            else
                window.DrawGrid(m_pos, m_size, 1, 1, m_sColor);
            if(axis == Horizontal)
                window.DrawRect(m_pos, {Math::Lerp<Vector2f>({0.f,0.f}, m_size, m_value / m_max).x, (float)m_size.y}, m_activeColor);
            else
                window.DrawRect(m_pos + Vector2i(0, Math::Lerp<Vector2f>({0,0}, m_size, 1.0 - m_value / m_max).y),
                               Vector2f(m_size.x, m_size.y - Math::Lerp<Vector2f>({0,0}, m_size, 1.0 - m_value / m_max).y), m_activeColor);
        }
        template class Slider<Vertical>;
        template class Slider<Horizontal>;
    }
}