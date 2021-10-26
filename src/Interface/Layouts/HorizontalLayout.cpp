#include <TML/Interface/Layouts/HorizontalLayout.h>
#include <TML/Utilities/Utilities.h>

namespace tml
{
    namespace Interface
    {
        HorizontalLayout::HorizontalLayout(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(w,h);
        }
        void HorizontalLayout::OnUpdate(float dt)
        {
            float offset = 0;
            const float width = (m_size.x - 5) / m_children.size();
            for(auto item = m_children.rbegin(); item != m_children.rend(); ++item)
            {
                auto* i = item->second;
                i->SetPosition({m_pos.x + offset, m_pos.y});
                if(i->GetVerticalSizePolicy() == Expand)
                    i->SetSize({Util::Clamp<float>(i->GetSize().x, 0, m_size.x - offset), m_size.y});
                if(i->GetHorizontalSizePolicy() == Expand)
                    i->SetSize({width, i->GetSize().y});
                offset += i->GetSize().x + 5; // 5 is here for padding
            }
        }
    }
}