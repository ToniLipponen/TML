#include <TML/Interface/Layouts/Vertical.h>

namespace tml::Interface
{
    void VerticalLayout::UpdateComponents()
    {
        Vector2 m_offset = {1, 0};
        for(auto i : m_children)
        {
            i.second->SetPosition(m_pos + m_offset);
            if(i.second->GetHorizontalSizePolicy() == BaseComponent::SizePolicy::Expand)
                i.second->SetSize({m_size.x - 2, i.second->GetSize().y});
            m_offset.y += i.second->GetSize().y + 4; // 4 is for padding
        }
    }

    void VerticalLayout::Draw()
    {

    }
}