#include <TML/Interface/Components/Label.h>
#include <TML/Renderer.h>

extern tml::Text* DEFAULT_TEXT;

namespace tml
{
    namespace Interface
    {

        Label::Label(i32 x, i32 y, ui32 s, std::string str)
        : m_text(std::move(str))
        {
            m_pos.x = x;
            m_pos.y = y;
            m_fontSize = s;

            DEFAULT_TEXT->SetString(m_text);
            DEFAULT_TEXT->SetSize(m_fontSize);
            m_size = DEFAULT_TEXT->GetDimensions();
            m_pColor = BLACK;
        }

        void Label::SetValue(std::string str)
        {
            m_text = std::move(str);
            DEFAULT_TEXT->SetString(m_text);
            DEFAULT_TEXT->SetSize(m_fontSize);
            m_size = DEFAULT_TEXT->GetDimensions();
        }

        bool Label::ContainsPoint(const Vector2 &p)
        {
            DEFAULT_TEXT->SetString(m_text);
            DEFAULT_TEXT->SetSize(m_fontSize);
            return (p > m_pos && p < (m_pos + m_size));
        }

        void Label::Draw()
        {
            Renderer::DrawText(m_text, m_pos, m_fontSize, m_pColor);
        }
    }
}