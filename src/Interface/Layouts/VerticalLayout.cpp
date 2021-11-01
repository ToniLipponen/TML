#include <TML/Interface/Layouts/VerticalLayout.h>
#include <TML/Utilities/Utilities.h>
#include <TML/Renderer.h>

namespace tml
{
    namespace Interface
    {
        VerticalLayout::VerticalLayout(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(w,h);
            m_hSizePolicy = Expand;
            m_vSizePolicy = Expand;
        }

        void VerticalLayout::OnUpdate(double dt)
        {
            float offset = 0;
            for(auto item = m_children.rbegin(); item != m_children.rend(); ++item)
            {
                auto* i = item->second;
                i->SetPosition({m_pos.x, m_pos.y + offset});
                if(i->GetHorizontalSizePolicy() == Expand)
                    i->SetSize({m_size.x, i->GetSize().y});
                offset += i->GetSize().y + 5; // 5 is here for padding
            }
        }

        void VerticalLayout::Draw()
        {
            Renderer::DrawRect(m_pos, m_size, m_pColor);
        }
    }
}