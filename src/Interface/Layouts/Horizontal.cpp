#include <TML/Interface/Layouts/Horizontal.h>

namespace tml::Interface
{
    void HorizontalLayout::UpdateComponents()
    {
        Vector2 m_offset = {0, 1};
        for(auto i : m_components)
        {
            i->SetPosition(m_pos + m_offset);
            if(i->GetHorizontalSizePolicy() == BaseComponent::SizePolicy::Expand)
                i->SetSize({m_size.x - 2, i->GetSize().y});
            if(i->GetVerticalSizePolicy() == BaseComponent::SizePolicy::Expand)
                i->SetSize({i->GetSize().x, m_size.y - 2});
            m_offset.x += i->GetSize().x + 4; // 4 is for padding
        }
    }

    void HorizontalLayout::Draw() {}
}