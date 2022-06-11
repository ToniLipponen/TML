#include <TML/Interface/Components/Button.h>

using namespace tml::Interface;

Button::Button(const std::string& text, uint32_t w, uint32_t h, int32_t x, int32_t y, const EventCallback& onClick, bool expand)
: BaseComponent(x,y,w,h)
{
    m_hSizePolicy = expand ? SizePolicy::Expand : SizePolicy::Fixed;
    m_vSizePolicy = SizePolicy::Fixed;
    m_text.SetString(text);
    m_text.SetColor(Color::Black);

    m_text.SetSize(static_cast<float>(h) * 0.6f);

    if(w == 0)
    {
        m_size.x = m_text.GetDimensions().x + 10;
        m_size.y = h;
    }
    else
    {
        m_size = Vector2f(w, h);
    }

    m_originalSize = m_size;

    const Vector2i textSize = m_text.GetDimensions();
    m_text.SetPosition(m_pos + (m_size / 2) - (textSize / 2));

    AddListener("Click", [&](BaseComponent* c, Event& e)
    {
        UnFocus();
        e = Event{};
    });

    if(onClick)
    {
        AddListener("Click", onClick);
    }

    AddListener("MouseUp", [&](BaseComponent* c, Event& e)
    {
        if(!m_state.MouseOver)
        {
            UnFocus();
        }
    });

    AddListener("MouseDown",[&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            Focus();
            m_state.MouseDown = static_cast<char>(e.mouseButton.button);
            e = Event{};
        }
        else
        {
            UnFocus();
        }
    });

    AddListener("Moved", [&](BaseComponent* c, Event& e)
    {
        const Vector2i textSize = m_text.GetDimensions();
        m_text.SetPosition(m_pos + (m_size / 2) - (textSize / 2));
    });

    AddListener("Resized", [&](BaseComponent* c, Event& e)
    {
        m_text.SetSize(m_size.y * 0.6f);
        const Vector2i textSize = m_text.GetDimensions();
        m_text.SetPosition(m_pos + (m_size / 2) - (textSize / 2));
    });

    AddListener("Drawn", [&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress + e.update.delta * 3, 0, 1);
        }
        else
        {
            m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress - e.update.delta * 1, 0, 1);
        }

        if(m_state.MouseDown > -1)
        {
            m_bodyAnimationProgress = 1;
        }
        else
        {
            m_bodyAnimationProgress = Math::Clamp<double>(m_bodyAnimationProgress - e.update.delta * 3, 0, 1);
        }

        m_borderColor = Math::Lerp(m_sColor, m_activeColor, m_borderAnimationProgress);
        m_bodyColor = Math::Lerp(m_pColor, m_activeColor, m_bodyAnimationProgress);
    });
}

void Button::SetRoundness(float roundness)
{
    m_roundness = roundness;
}

void Button::SetText(const std::string &str)
{
    m_text.SetString(str);
}

void Button::pDraw(RenderTarget& target) noexcept
{
    target.DrawRect(m_pos, m_size, m_borderColor, m_roundness);
    target.DrawRect(m_pos + Vector2f(1,1), m_size - Vector2f(2,2), m_bodyColor, m_roundness);

    target.SetBounds(m_pos, m_size);
    target.Draw(m_text);
    target.ResetBounds();
}