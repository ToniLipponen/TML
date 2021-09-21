#include <TML/Interface/Components/Listbox.h>
#include <TML/Renderer.h>

using namespace tml::Interface;

Listbox::Listbox(i32 x, i32 y, ui32 width, ui32 height)
{
    m_absPos = Vector2(x,y);
    m_relPos = m_absPos;
    m_absSize = Vector2(width, height);
    m_relSize = m_absSize;
}

void Listbox::AddValue(std::string value)
{
    m_values.push_back(std::move(value));
}

void Listbox::SetValue(ui32 index, std::string value)
{
    if(index > m_values.size() - 1)
        return;
    else
        m_values.at(index) = std::move(value);
}

std::string Listbox::GetValue(ui32 index)
{
    if(index >= m_values.size())
        return "";
    else
        return m_values.at(index);
}

std::string Listbox::GetSelectedValue() const
{
    if(m_selectedIndex != -1 && m_selectedIndex < m_values.size())
    {
        return m_values.at(m_selectedIndex);
    }
    return "";
}

tml::i32 Listbox::GetSelectedIndex() const
{
    return m_selectedIndex;
}

void Listbox::Clear()
{
    m_values.clear();
}

void Listbox::OnMouseClick(const Vector2 &mousePos)
{
    auto PointInRect = [&](const Vector2& tl, const Vector2& br)
    {
        return (mousePos < br && mousePos > tl);
    };
    for(int i = 0; i < m_values.size(); i++)
    {
        if(PointInRect(m_absPos + Vector2(0, i * 20),
                       m_absPos + Vector2(m_absSize.x - 20, (i * 20) + 20)))
        {
            m_selectedIndex = i;
            break;
        }
    }
}

void Listbox::OnUpdate(float dt)
{

}

void Listbox::Draw()
{
    Renderer::DrawRect(m_absPos, m_absSize, m_pColor);
    for(int i = 0; i < m_values.size(); i++)
    {
        if(i == m_selectedIndex)
            Renderer::DrawRect(m_absPos + Vector2(0, i * 20 - m_scrollDist), {m_absSize.x, 20.f}, m_activeColor);
        Renderer::DrawTextCropped(m_values.at(i), m_absPos + Vector2(5, i * 20 - m_scrollDist),20,
                                  BLACK, m_absPos, m_absPos + m_absSize);
    }
    if(ActiveComponent == this)
        Renderer::DrawGrid(m_absPos, m_absSize, 1, 1, m_activeColor, 2);
    else
        Renderer::DrawGrid(m_absPos, m_absSize, 1, 1, m_sColor, 2);
}