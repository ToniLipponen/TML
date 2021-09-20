#include <TML/Interface/Components/Checkbox.h>
#include <TML/Renderer.h>
using namespace tml::Interface;

Checkbox::Checkbox(i32 x, i32 y, ui32 size, bool checked)
{
    m_absPos = Vector2(x,y);
    m_absSize = Vector2(size,size);
    m_value = checked;
}

void Checkbox::Draw()
{
    Renderer::DrawRect(m_absPos, m_absSize, BLUE, 8.f);
    Renderer::DrawRect(m_absPos + m_absSize*0.05f, m_absSize*0.9f, m_pColor, 8.f);
    if(m_value)
        Renderer::DrawRect(m_absPos + m_absSize*0.2f, m_absSize*0.6f, BLUE, 4.f);
}

void Checkbox::OnMouseClick(const Vector2& mousePos)
{
    m_value = !m_value;
}