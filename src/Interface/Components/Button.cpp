#include <TML/Interface/Components/Button.h>
#include <TML/Graphics/Renderer.h>
#include <iostream>

using namespace tml::Interface;
extern tml::Text* DEFAULT_TEXT;

Button::Button(ui32 x, ui32 y, ui32 w, ui32 h, std::string str, UIFunc onClick)
: m_text(std::move(str)), m_tColor(BLACK)
{
    m_size = Vector2i(w,h);
    m_pos = Vector2i(x,y);
    if(onClick)
        AddListener("Click", onClick);

    AddListener("iClick", [&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver || (m_state.MouseDown != -1))
        {
            m_state.MouseDown = e.mouseButton.button;
            UnFocus();
            e = Event{};
            return true;
        }
        return false;
    });

    AddListener("iMouseDown",[&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            Focus();
            m_state.MouseDown = e.mouseButton.button;
            e = Event{};
            return true;
        }
        UnFocus();
        return false;
    });
}

void Button::SetText(const std::string &str)
{
    m_text = str;
}

void Button::Draw()
{
    DEFAULT_TEXT->SetString(m_text);
    DEFAULT_TEXT->SetSize(m_textSize);
    const Vector2i textSize = DEFAULT_TEXT->GetDimensions();

    if(m_state.Focused)
        Renderer::DrawRect(m_pos, m_size, m_activeColor);
    else
        Renderer::DrawRect(m_pos, m_size, m_pColor);

    Renderer::SetBounds(m_pos, m_size);
    Renderer::DrawText(m_text, m_pos + (m_size / 2) - (textSize / 2), m_textSize, m_tColor);
    Renderer::ResetBounds();

    if(m_state.Focused)
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor,1);
    else
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor,1);
}