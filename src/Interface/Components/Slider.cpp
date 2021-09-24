#include <TML/Interface/Components/Slider.h>
#include <TML/Utilities/Utilities.h>
#include <TML/Renderer.h>

// Sets a given bit
#define SETBIT(value, bit, state) (value ^= (-state ^ value) & (1UL << bit))
// Checks whether a bit is set or not
#define BITSET(value, bit) ((value & bit) > 0)
using namespace tml::Interface;

Slider::Slider(ui32 type, i32 x, i32 y, ui32 size, ui32 thickness, float min, float max)
{
    m_min = Util::Min<float>(min, 0);
    m_max = Util::Min<float>(max, 0);
    m_value = (m_min + m_max) / 2;
    m_type = type;
    m_thickness = thickness;
    m_size = size;

    if(type == Horizontal)
    {
        m_relPos = Vector2(x,y);
        m_absPos = m_relPos;
        m_absSize = Vector2(size, thickness);
    }
    else
    {
        m_relPos = Vector2(x,y);
        m_absPos = m_relPos;
        m_absSize = Vector2(thickness, size);
    }
}

void Slider::SetValue(float value)
{
    m_value = Util::Clamp(value, m_min, m_max);
}

void Slider::SetSize(const Vector2 &size)
{
    if(m_type == Horizontal)
        m_absSize = {size.x, size.y};
    else
        m_absSize = {size.x, size.y};
}

void Slider::OnMouseDown(const Vector2& mp)
{
    if(m_type == Horizontal)
        m_value = (mp.x - m_absPos.x) / m_absSize.x * m_max;
    else
        m_value = m_max - ((mp.y - m_absPos.y) / m_absSize.y * m_max);
}
void Slider::OnUpdate(float dt)
{

}

void Slider::Draw()
{
    if(m_type == Horizontal)
    {
        const Vector2 a = m_absPos + Vector2{m_thickness / 4, m_thickness / 2};
        const Vector2 b = a + Vector2(m_size - m_thickness / 2, 0);
        Renderer::DrawLine(a,b,(m_thickness / 2) * 1.1f,m_sColor);
        Renderer::DrawLine(a,b,m_thickness / 2,m_pColor);
        Renderer::DrawLine(a,Util::Lerp(a,b,m_value / m_max),m_thickness / 2,m_activeColor);
        if(m_state.Focused)
            Renderer::DrawCircle(Util::Lerp(a, b, m_value / m_max),m_thickness / 2,m_activeColor);
        else
            Renderer::DrawCircle(Util::Lerp(a, b, m_value / m_max),m_thickness / 2,m_sColor);
        Renderer::DrawCircle(Util::Lerp(a, b, m_value / m_max), (m_thickness / 2) * 0.85f,m_pColor);
    }
    else
    {
        const Vector2 a = m_absPos + Vector2(m_thickness / 2, m_thickness / 4);
        const Vector2 b = a + Vector2(0, m_size - m_thickness / 2);
        Renderer::DrawLine(a,b,(m_thickness / 2) *1.1f,m_sColor);
        Renderer::DrawLine(a, b,m_thickness / 2,m_pColor);
        Renderer::DrawLine(b, Util::Lerp(a, b, 1.f - (m_value / m_max)),m_thickness / 2,m_activeColor);
        if(m_state.Focused)
            Renderer::DrawCircle(Util::Lerp(a, b, 1.f - (m_value / m_max)), m_thickness / 2, m_activeColor);
        else
            Renderer::DrawCircle(Util::Lerp(a, b, 1.f - (m_value / m_max)),m_thickness / 2,m_sColor);
        Renderer::DrawCircle(Util::Lerp(a, b, 1.f - (m_value / m_max)),(m_thickness / 2) * 0.85f,m_pColor);
    }
}