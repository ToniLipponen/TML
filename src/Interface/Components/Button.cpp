#include <TML/Interface/Components/Button.h>
#include <TML/Graphics/Renderer.h>
#include <iostream>

using namespace tml::Interface;
extern tml::Text* DEFAULT_TEXT;

Button::Button(ui32 x, ui32 y, ui32 w, ui32 h, std::string str, Function_ptr<void, BaseComponent*> onClick)
: m_text(std::move(str)), m_tColor(BLACK)
{
    m_size = Vector2(w,h);
    m_pos = Vector2(x,y);
    m_onClickFunc = onClick;
}

void Button::SetText(const std::string &str)
{
    m_text = str;
}

void Button::Draw()
{
    DEFAULT_TEXT->SetString(m_text);
    DEFAULT_TEXT->SetSize(m_textSize);
    Vector2 textSize = DEFAULT_TEXT->GetDimensions();

    const Vector2 pos = m_pos + Vector2(1,1);
    const Vector2 size = m_size - Vector2(2,2);

    if(m_eventStatus.MouseDown)
        Renderer::DrawRect(m_pos, m_size, m_activeColor);
    else
        Renderer::DrawRect(m_pos, m_size, m_pColor);

    Renderer::DrawText(m_text, pos + (size / 2.f) - (textSize / 2.f), m_textSize, m_tColor);

    if(m_state.Focused)
        Renderer::DrawGrid(pos, size, 1, 1, m_activeColor,1);
    else
        Renderer::DrawGrid(pos, size, 1, 1, m_sColor,1);
}