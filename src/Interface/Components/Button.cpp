#include <TML/Interface/Components/Button.h>
#include <TML/Graphics/Renderer.h>
#include <iostream>

using namespace tml::Interface;

Button::Button(i32 x, i32 y, ui32 w, ui32 h, std::string str, UIFunc onClick)
: BaseComponent(x,y,w,h)
{
    m_hSizePolicy = SizePolicy::Expand;
    m_vSizePolicy = SizePolicy::Clamp;
    m_text.SetString(str);
    m_text.SetSize(h*0.6);
    m_text.SetColor(BLACK);

    const Vector2i textSize = m_text.GetDimensions();
    m_text.SetPosition(m_pos + (m_size / 2) - (textSize / 2));
    if(onClick)
        AddListener("Click", std::move(onClick));

    AddListener("Click", [&](BaseComponent* c, Event& e)
    {
        UnFocus();
    });

    AddListener("MouseUp", [&](BaseComponent* c, Event& e)
    {
        if(!m_state.MouseOver)
            UnFocus();
    });

    AddListener("MouseDown",[&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            Focus();
            m_state.MouseDown = e.mouseButton.button;
        }
        else
            UnFocus();
    });

    AddListener("Moved", [&](BaseComponent* c, Event& e)
    {
        const Vector2i textSize = m_text.GetDimensions();
        m_text.SetPosition(m_pos + (m_size / 2) - (textSize / 2));
    });
}

void Button::SetText(const std::string &str)
{
    m_text.SetString(str);
}

void Button::Draw()
{
    if(m_state.Focused)
        Renderer::DrawRect(m_pos, m_size, m_activeColor);
    else
        Renderer::DrawRect(m_pos, m_size, m_pColor);

    Renderer::SetBounds(m_pos, m_size);
    Renderer::Draw(m_text);
    Renderer::ResetBounds();

    if(m_state.Focused)
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor,1);
    else
        Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor,1);
}