#include <TML/Interface/Components/Slider.h>

namespace tml::Interface
{
    template<ComponentAxis axis>
    Slider<axis>::Slider(int32_t x, int32_t y, uint32_t size, uint32_t thickness, float min, float max) noexcept
    {
        m_min = Math::Max<float>(min, 0);
        m_max = Math::Max<float>(max, 0);
        m_value = (m_min + m_max) / 2;

        if(axis == ComponentAxis::Horizontal)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(size, thickness);
            m_vSizePolicy = SizePolicy::Fixed;
            m_hSizePolicy = SizePolicy::Expand;
        }
        else
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(thickness, size);
            m_vSizePolicy = SizePolicy::Expand;
            m_hSizePolicy = SizePolicy::Fixed;
        }

        AddListener("MouseDown", [&](BaseComponent* c, Event& e)
        {
            if(m_state.MouseOver)
            {
                m_state.MouseDown = static_cast<char>(e.mouseButton.button);
                if constexpr(axis == ComponentAxis::Horizontal)
                {
                    m_value = Math::Clamp(float(e.mouseButton.x - m_pos.x) / float(m_size.x) * m_max, m_min, m_max);
                }
                else
                {
                    m_value = Math::Clamp(m_max - float((e.mouseButton.y - m_pos.y) / float(m_size.y) * m_max), m_min, m_max);
                }

                e = {};
            }
        });

        AddListener("MouseMoved", [&](BaseComponent* c, Event& e)
        {
            if(m_state.MouseDown != -1)
            {
                if constexpr(axis == ComponentAxis::Horizontal)
                {
                    m_value = Math::Clamp(float(e.pos.x - m_pos.x) / float(m_size.x) * m_max, m_min, m_max);
                }
                else
                {
                    m_value = Math::Clamp(m_max - float((e.pos.y - m_pos.y) / float(m_size.y) * m_max), m_min, m_max);
                }
            }
        });
    }

    template<ComponentAxis axis>
    void Slider<axis>::SetRoundness(float roundness) noexcept
    {
        m_roundness = roundness;
    }

    template<ComponentAxis axis>
    void Slider<axis>::SetValue(float value) noexcept
    {
        m_value = Math::Clamp(value, m_min, m_max);
    }

    template<ComponentAxis axis>
    float Slider<axis>::GetValue() const noexcept
    {
        return m_value;
    }

    template<ComponentAxis axis>
    void Slider<axis>::pDraw(RenderTarget& target) noexcept
    {
        target.DrawRect(m_pos, m_size, m_state.MouseOver || m_state.MouseDown > -1 ? m_activeColor : m_sColor, m_roundness);
        target.DrawRect(m_pos + Vector2i(1, 1), m_size - Vector2i(2, 2), m_pColor, m_roundness);

        if constexpr(axis == ComponentAxis::Horizontal)
        {
            const auto x = Math::Max<float>(Math::Lerp<float>(0, m_size.x, m_value / m_max), m_roundness*2);
            target.DrawRect(m_pos + Vector2f(1, 1), Vector2f(x - 2 , m_size.y - 2), m_activeColor, m_roundness);
        }
        else
        {
            const auto y = Math::Min<float>(Math::Lerp<float>(0, m_size.y, 1.0 - m_value / m_max), m_size.y - m_roundness * 2);
            target.DrawRect(m_pos + Vector2i(0, y), Vector2f(m_size.x, m_size.y - y), m_activeColor, m_roundness);
        }
    }

    template class Slider<ComponentAxis::Vertical>;
    template class Slider<ComponentAxis::Horizontal>;
}