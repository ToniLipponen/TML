#include <TML/Interface/Layouts/Horizontal.h>

namespace tml::Interface
{
    void HorizontalLayout::UpdateComponents()
    {
        Vector2 m_offset = {0, 1};
        for(auto i : m_children)
        {
            i.second->SetPosition(m_pos + m_offset);
            if(i.second->GetHorizontalSizePolicy() == BaseComponent::SizePolicy::Expand)
                i.second->SetSize({m_size.x - 2, i.second->GetSize().y});
            if(i.second->GetVerticalSizePolicy() == BaseComponent::SizePolicy::Expand)
                i.second->SetSize({i.second->GetSize().x, m_size.y - 2});
            m_offset.x += i.second->GetSize().x + 4; // 4 is for padding
        }
    }

    void HorizontalLayout::Draw() {}
}