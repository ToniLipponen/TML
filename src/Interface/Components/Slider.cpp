#include <TML/Interface/Components/Slider.h>

namespace tml::Interface
{
    template<ComponentAxis axis>
    Slider<axis>::Slider(uint32_t size, uint32_t thickness, int32_t x, int32_t y, float min, float max) noexcept
    {
        m_min = Math::Max<float>(min, 0);
        m_max = Math::Max<float>(max, 0);
        m_minimumSize = {thickness};
        m_value = (m_min + m_max) / 2;
        m_roundness = thickness / 2;

        if constexpr(axis == ComponentAxis::Horizontal)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(size, thickness);
            m_vSizePolicy = SizePolicy::Fixed;
            m_hSizePolicy = SizePolicy::Expand;

            if(size)
            {
                m_minimumSize.x = static_cast<float>(size);
            }
        }
        else
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(static_cast<int>(thickness), static_cast<int>(size));
            m_vSizePolicy = SizePolicy::Expand;
            m_hSizePolicy = SizePolicy::Fixed;

            if(size)
            {
                m_minimumSize.y = static_cast<float>(size);
            }
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

        AddListener("Click", [&](BaseComponent* c, Event& e)
        {
            if constexpr(axis == ComponentAxis::Horizontal)
            {
                m_value = Math::Clamp(float(e.mouseButton.x - m_pos.x) / float(m_size.x) * m_max, m_min, m_max);
            }
            else
            {
                m_value = Math::Clamp(m_max - float((e.mouseButton.y - m_pos.y) / float(m_size.y) * m_max), m_min, m_max);
            }

            Focus();
            Raise();
            m_targetValue = m_value;
            e = {};
        });

        AddListener("MouseDown", [&](BaseComponent* c, Event& e)
        {
            if(m_state.MouseOver)
            {
                m_state.MouseDown = static_cast<char>(e.mouseButton.button);
                e = Event{};
            }
            else
            {
                UnFocus();
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

                m_targetValue = m_sliderValue = m_value;
            }
        });

        AddListener("KeyPressed", [&](BaseComponent* c, Event& e)
        {
            if(m_state.Focused)
            {
                switch(e.key.value)
                {
                    case Keyboard::Key::LeftArrow:
                    case Keyboard::Key::DownArrow:
                        m_targetValue = Math::Clamp<float>(m_targetValue -= 0.1, 0, 1);
                        break;

                    case Keyboard::Key::RightArrow:
                    case Keyboard::Key::UpArrow:
                        m_targetValue = Math::Clamp<float>(m_targetValue += 0.1, 0, 1);
                        break;

                    default:
                        break;
                }
            }
        });

        AddListener("Drawn", [&](BaseComponent* c, Event& e)
        {
            if(m_state.MouseOver)
            {
                m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress + e.update.delta * s_animationSpeed, 0, 1);
            }
            else
            {
                m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress - e.update.delta * s_animationSpeed, 0, 1);
            }

            if(m_sliderValue != m_targetValue)
            {
                if(m_sliderValue < m_targetValue)
                {
                    m_sliderValue += e.update.delta * s_animationSpeed;
                    m_sliderValue = Math::Clamp<float>(m_sliderValue, 0, m_targetValue);
                }
                else
                {
                    m_sliderValue -= e.update.delta * s_animationSpeed;
                    m_sliderValue = Math::Clamp<float>(m_sliderValue, m_targetValue, 1);
                }
            }

            if(m_state.Focused)
            {
                m_borderAnimationProgress = 1;
            }

            m_borderColor = Math::Lerp(m_sColor, m_activeColor, m_borderAnimationProgress);
        });
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
        target.DrawRect(m_pos, m_size, m_borderColor, m_roundness);
        target.DrawRect(m_pos + Vector2i(1, 1), m_size - Vector2i(2, 2), m_pColor, m_roundness);

        if constexpr(axis == ComponentAxis::Horizontal)
        {
            const auto x = Math::Max<float>(Math::Lerp<float>(0, m_size.x, m_sliderValue / m_max), m_roundness*2);
            target.DrawRect(m_pos + Vector2f(1, 1), Vector2f(x - 2 , m_size.y - 2), m_activeColor, m_roundness);
        }
        else
        {
            const auto y = Math::Min<float>(Math::Lerp<float>(0, m_size.y, 1.0 - m_sliderValue / m_max), m_size.y - m_roundness * 2);
            target.DrawRect(m_pos + Vector2i(0, y), Vector2f(m_size.x, m_size.y - y), m_activeColor, m_roundness);
        }
    }

    template class Slider<ComponentAxis::Vertical>;
    template class Slider<ComponentAxis::Horizontal>;
}