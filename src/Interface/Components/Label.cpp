#include <TML/Interface/Components/Label.h>
#include <TML/Graphics/Renderer.h>
#include "TML/Utilities/Utilities.h"

namespace tml
{
    namespace Interface
    {
        Label::Label(i32 x, i32 y, ui32 s, std::string str)
        {
            m_pos.x = x;
            m_pos.y = y;
            m_text.SetString(str);
            m_text.SetSize(s);
            m_text.SetColor(BLACK);
            m_size = m_text.GetDimensions();
            AddListener("Moved", [&](BaseComponent*, Event& e)
            {
                m_text.SetPosition(e.move.x, e.move.y);
            });
        }

        void Label::SetValue(std::string str)
        {
            m_text.SetString(str);
            m_size = m_text.GetDimensions();
        }

        bool Label::ContainsPoint(const Vector2i &p)
        {
            return Util::PointInRect(p, m_pos, m_size, 0);
        }

        void Label::Draw()
        {
            Renderer::Draw(m_text);
        }
    }
}