#include <TML/Interface/Components/Button.h>

using namespace tml::Interface;

Button::Button(const std::string& text, i32 x, i32 y, ui32 w, ui32 h, const UIFunc& onClick, bool expand)
: BaseComponent(x,y,w,h)
{
    m_hSizePolicy = expand ? SizePolicy::Expand : SizePolicy::Clamp;
    m_vSizePolicy = SizePolicy::Clamp;
    m_text.SetString(text);
    m_text.SetColor(Color::Black);
    if(h == 0)
    {
        m_text.SetSize(20);
        m_size = m_text.GetDimensions() + Vector2f(10, 0);
        m_originalSize = m_size;
    }
    else
    {
        m_text.SetSize(h*0.8f);
    }

    const Vector2i textSize = m_text.GetDimensions();
    m_text.SetPosition(m_pos + (m_size / 2) - (textSize / 2));

    AddListener("Click", [&](BaseComponent* c, Event& e)
    {
        UnFocus();
        e = Event{};
    });

    if(onClick)
        AddListener("Click", onClick);

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
            e = Event{};
        }
        else
            UnFocus();
    });

    AddListener("Moved", [&](BaseComponent* c, Event& e)
    {
        const Vector2i textSize = m_text.GetDimensions();
        m_text.SetPosition(m_pos + (m_size / 2) - (textSize / 2));
    });

    AddListener("Resized", [&](BaseComponent* c, Event& e)
    {
        m_text.SetSize(e.size.h*0.8);
        const Vector2i textSize = m_text.GetDimensions();
        m_text.SetPosition(m_pos + (m_size / 2) - (textSize / 2));
    });
}


void Button::SetText(const std::string &str)
{
    m_text.SetString(str);
}

void Button::pDraw(Renderer &window)
{
    if(m_state.MouseDown == -1)
        window.DrawRect(m_pos, m_size, m_pColor);
    else
        window.DrawRect(m_pos, m_size, m_activeColor);

    window.SetBounds(m_pos, m_size);
    window.Draw(m_text);
    window.ResetBounds();

    if(m_state.MouseOver)
        window.DrawGrid(m_pos, m_size, 1, 1, m_activeColor,1);
    else
        window.DrawGrid(m_pos, m_size, 1, 1, m_sColor,1);
}