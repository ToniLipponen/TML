#include <TML/Interface/Components/Checkbox.h>

namespace tml::Interface
{
    Checkbox::Checkbox(uint32_t size, int32_t x, int32_t y, bool checked) noexcept
    {
        m_pos = Vector2f(x,y);
        m_size = Vector2f(size,size);
        m_value = checked;

        AddListener("Click", [&](Component*, const Event& e)
        {
            m_value = !m_value;
            return true;
        });

        AddListener("Drawn", [&](Component* c, const Event& e)
        {
            if(m_state.MouseOver || m_state.Focused || m_state.Dragged)
            {
                m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress + e.update.delta * s_animationSpeed, 0, 1);
            }
            else
            {
                m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress - e.update.delta * s_animationSpeed, 0, 1);
            }

            if(m_value)
            {
                m_bodyAnimationProgress = Math::Clamp<double>(m_bodyAnimationProgress + e.update.delta * s_animationSpeed, 0, 1);
            }
            else
            {
                m_bodyAnimationProgress = Math::Clamp<double>(m_bodyAnimationProgress - e.update.delta * s_animationSpeed, 0, 1);
            }

            m_borderColor = Math::Lerp(m_sColor, m_activeColor, m_borderAnimationProgress);
            m_bodyColor = Math::Lerp(m_pColor, m_activeColor, m_bodyAnimationProgress);
            return true;
        });
    }

    bool Checkbox::GetValue() const noexcept
    {
        return m_value;
    }


    void Checkbox::SetValue(bool value) noexcept
    {
        m_value = value;
    }

    void Checkbox::pDraw(RenderTarget& target) noexcept
    {
        target.DrawRect(m_pos, m_size, m_borderColor, m_roundness);
        target.DrawRect(m_pos+Vector2i(1,1), m_size-Vector2i(2,2), m_pColor, m_roundness);
        target.DrawRect(m_pos + m_size * 0.2f, m_size * 0.6f, m_bodyColor, m_roundness);
    }
}