#include <TML/Interface/Layouts/Vertical.h>

namespace tml::Interface
{
    void VerticalLayout::UpdateComponents()
    {
        Vector2 m_offset = {0};
        for(auto i : m_components)
        {
            i->SetPosition(m_pos + m_offset);
            if(i->GetHorizontalSizePolicy() == BaseComponent::SizePolicy::Expand)
                i->SetSize({m_size.x, i->GetSize().y});
            m_offset.y += i->GetSize().y + 4; // 4 is for padding
        }
    }
}