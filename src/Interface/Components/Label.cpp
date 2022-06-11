#include <TML/Interface/Components/Label.h>

namespace tml::Interface
{
    Label::Label(const std::string& labelText, uint32_t s, int32_t x, int32_t y) noexcept
    {
        m_pos.x = x;
        m_pos.y = y;
        m_text.SetString(labelText);
        m_text.SetSize(s);
        m_text.SetColor(Color::Black);
        m_size = m_text.GetDimensions();

        AddListener("Moved", [&](BaseComponent*, Event& e) noexcept
        {
            m_text.SetPosition(m_pos);
        });
    }

    void Label::SetValue(std::string str) noexcept
    {
        m_text.SetString(str);
        m_size = m_text.GetDimensions();
    }

    bool Label::ContainsPoint(const Vector2i &p) noexcept
    {
        return Math::PointInRect(p, m_pos, m_size, 0);
    }

    void Label::pDraw(RenderTarget& target) noexcept
    {
        target.Draw(m_text);
    }
}