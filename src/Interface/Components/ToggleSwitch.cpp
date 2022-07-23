#include <TML/Interface/Components/ToggleSwitch.h>

namespace tml::Interface
{
    ToggleSwitch::ToggleSwitch(int32_t x, int32_t y, uint32_t size, bool state) noexcept
    : Component(x, y, size * 2, size), m_toggleState(state)
    {
        m_roundness = 30;

        AddListener("Click", [&](Component*, const Event&)
        {
            m_toggleState = !m_toggleState;
            return true;
        });

        AddListener("Drawn", [&](Component*, const Event& e)
        {
            if(m_toggleState)
            {
                m_knobValue += e.update.delta * s_animationSpeed;
            }
            else
            {
                m_knobValue -= e.update.delta * s_animationSpeed;
            }

            if(m_state.MouseOver || m_state.Focused || m_state.Dragged)
            {
                m_borderAnimationProgress += e.update.delta * s_animationSpeed;
            }
            else
            {
                m_borderAnimationProgress -= e.update.delta * s_animationSpeed;
            }

            m_knobValue = Math::Clamp<float>(m_knobValue, 0, 1);
            m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress, 0, 1);
            m_borderColor = Math::Lerp(m_sColor, m_activeColor, m_borderAnimationProgress);
            return true;
        });
    }

    bool ToggleSwitch::GetValue() const noexcept
    {
        return m_toggleState;
    }

    void ToggleSwitch::SetValue(bool value) noexcept
    {
        m_toggleState = value;
    }

    void ToggleSwitch::pDraw(RenderTarget &target) noexcept
    {
        const auto clampedRadius = Math::Clamp<float>(m_roundness, 0, m_size.y / 2);
        target.DrawRect(m_pos, m_size, m_borderColor, clampedRadius);
        target.DrawRect(m_pos + Vector2f(1,1), m_size - Vector2f(2,2), m_pColor, clampedRadius);

        target.DrawRect(m_pos + Vector2f(2,2), Vector2f(Math::Max(m_size.x * m_knobValue - 4, m_size.y - 4), m_size.y - 4), m_activeColor, clampedRadius);
        target.DrawRect(m_pos + Vector2f(2,2) + Vector2f(m_knobValue * m_size.y,0), Vector2f(m_size.y, m_size.y) - Vector2f(4,4), m_sColor, clampedRadius);
        target.DrawRect(m_pos + Vector2f(3,3) + Vector2f(m_knobValue * m_size.y,0), Vector2f(m_size.y, m_size.y) - Vector2f(6,6), m_pColor, clampedRadius);
    }
}