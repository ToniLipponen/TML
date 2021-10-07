#include <TML/Interface/Components/Label.h>
#include <TML/Renderer.h>

extern tml::Text* DEFAULT_TEXT;

namespace tml::Interface
{
    Label::Label(i32 x, i32 y, ui32 s, std::string str)
    : m_text(std::move(str))
    {
        m_pos.x = x;
        m_pos.y = y;
        m_size = s;
    }

    void Label::SetValue(std::string str)
    {
        m_text = std::move(str);
    }

    bool Label::ContainsPoint(const Vector2 &p)
    {
        DEFAULT_TEXT->SetString(m_text);
        DEFAULT_TEXT->SetSize(m_size);
        const auto bounds = DEFAULT_TEXT->GetDimensions();
        return (p > m_pos && p < (m_pos + bounds));
    }

    void Label::Draw()
    {
        Renderer::DrawText(m_text, m_pos, m_size, m_pColor);
    }
}