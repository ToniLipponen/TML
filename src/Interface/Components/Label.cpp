#include <TML/Interface/Components/Label.h>

namespace tml::Interface
{
    Label::Label(const String& labelText, uint32_t s, uint32_t width, int32_t x, int32_t y) noexcept
    {
        m_pos = Vector2f(x, y);
        m_text.SetString(labelText);
        m_text.SetSize(s);
        m_text.SetColor(s_defaultTextColor);
        m_size = m_text.GetDimensions();

        if(width != 0)
        {
            m_size.x = width;
        }

        AddListener("Moved", [&](Component*, const Event& e) noexcept
        {
            m_text.SetPosition(m_pos);
            return true;
        });
    }

    void Label::SetValue(const String& str) noexcept
    {
        m_text.SetString(str);
        SetSize(m_text.GetDimensions());

        if(m_parent)
        {
            m_parent->SetPosition(m_parent->GetPosition());
        }
    }

    bool Label::ContainsPoint(const Vector2i &p) noexcept
    {
        return Math::PointInRect(p, m_pos, m_size, 0);
    }

    void Label::SetTextColor(const Color& color) noexcept
    {
        m_text.SetColor(color);
    }

    void Label::pDraw(RenderTarget& target) noexcept
    {
        target.Draw(m_text);
    }
}