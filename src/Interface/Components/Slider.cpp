#include <TML/Interface/Components/Slider.h>
#include <TML/Utilities/Utilities.h>
#include <TML/Renderer.h>

namespace tml::Interface
{
    Slider::Slider(ui32 type, i32 x, i32 y, ui32 size, ui32 thickness, float min, float max)
    {
        m_min = Util::Min<float>(min, 0);
        m_max = Util::Min<float>(max, 0);
        m_value = (m_min + m_max) / 2;
        m_type = type;
        if(type == Horizontal)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(size, thickness);
            m_vSizePolicy = Fixed;
            m_hSizePolicy = Expand;
        }
        else
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(thickness, size);
            m_vSizePolicy = Expand;
            m_hSizePolicy = Fixed;
        }
    }

    void Slider::SetValue(float value)
    {
        m_value = Util::Clamp(value, m_min, m_max);
    }

    void Slider::OnMouseDown(const Vector2& mp)
    {
//        if(m_type == Horizontal)
//            m_value = (mp.x - m_pos.x) / m_size.x * m_max;
//        else
//            m_value = m_max - ((mp.y - m_pos.y) / m_size.y * m_max);
    }

    void Slider::OnMouseDrag(const Vector2& mp)
    {
        if(m_type == Horizontal)
            m_value = Util::Clamp((mp.x - m_pos.x) / m_size.x * m_max, m_min, m_max);
        else
            m_value = Util::Clamp(m_max - ((mp.y - m_pos.y) / m_size.y * m_max), m_min, m_max);
    }

    void Slider::Draw()
    {
        // Maybe clean this up at some point?
        if(m_type == Horizontal)
        {
            const Vector2 a = m_pos + Vector2{m_size.y / 4, m_size.y / 2};
            const Vector2 b = a + Vector2(m_size.x - m_size.y / 2, 0);
            Renderer::DrawLine(a,b,(m_size.y / 2) * 1.25f,m_sColor);
            Renderer::DrawLine(a,b,m_size.y / 2,m_pColor);
            Renderer::DrawLine(a,Util::Lerp(a,b,m_value / m_max),m_size.y / 2,m_activeColor);
            if(m_state.Focused)
                Renderer::DrawCircle(Util::Lerp(a, b, m_value / m_max),m_size.y / 2,m_activeColor);
            else
                Renderer::DrawCircle(Util::Lerp(a, b, m_value / m_max),m_size.y / 2,m_sColor);
            Renderer::DrawCircle(Util::Lerp(a, b, m_value / m_max), (m_size.y / 2) * 0.85f,m_pColor);
        }
        else
        {
            const Vector2 a = m_pos + Vector2(m_size.x / 2, m_size.x / 4);
            const Vector2 b = a + Vector2(0, m_size.y - m_size.x / 2);
            Renderer::DrawLine(a,b,(m_size.x / 2) * 1.25f,m_sColor);
            Renderer::DrawLine(a, b,m_size.x / 2,m_pColor);
            Renderer::DrawLine(b, Util::Lerp(a, b, 1.f - (m_value / m_max)),m_size.x / 2,m_activeColor);
            if(m_state.Focused)
                Renderer::DrawCircle(Util::Lerp(a, b, 1.f - (m_value / m_max)), m_size.x / 2, m_activeColor);
            else
                Renderer::DrawCircle(Util::Lerp(a, b, 1.f - (m_value / m_max)),m_size.x / 2,m_sColor);
            Renderer::DrawCircle(Util::Lerp(a, b, 1.f - (m_value / m_max)),(m_size.x / 2) * 0.85f,m_pColor);
        }
    }
}