#include <TML/Interface/Components/Button.h>
#include <TML/Renderer.h>
#include <iostream>

using namespace tml::Interface;
extern tml::Text* DEFAULT_TEXT;

Button::Button(ui32 x, ui32 y, ui32 w, ui32 h, std::string str)
: m_text(std::move(str))
{
    m_size = Vector2(w,h);
    m_pos = Vector2(x,y);
}

void Button::SetText(const std::string &str)
{
    m_text = str;
}

void Button::Draw()
{
    DEFAULT_TEXT->SetString(m_text);
    DEFAULT_TEXT->SetSize(m_textSize);
    Vector2 size = DEFAULT_TEXT->GetDimensions();
    if(m_eventStatus.MouseDown)
        Renderer::DrawRect(m_pos, m_size, m_activeColor);
    else
        Renderer::DrawRect(m_pos, m_size, m_pColor);
    Renderer::DrawText(m_text, m_pos + (m_size / 2.f) - (size / 2.f), m_textSize, BLACK);
    if(m_state.Focused)
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor,2);
    else
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor,2);
}