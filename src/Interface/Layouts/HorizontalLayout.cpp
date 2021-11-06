#include <TML/Interface/Layouts/HorizontalLayout.h>
#include <TML/Utilities/Utilities.h>
#include <vector>

namespace tml
{
    namespace Interface
    {
        HorizontalLayout::HorizontalLayout(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(w,h);
            m_hSizePolicy = Expand;
            m_vSizePolicy = Expand;
        }

        void HorizontalLayout::OnUpdate(double dt)
        {
            ScaleChildren();
            AlignChildren();
        }

        void HorizontalLayout::ScaleChildren()
        {
            std::vector<BaseComponent*> expandThese;
            ui32 expandedChildren = 0;
            float width = 0, expandSize;
            for(auto& item : m_children)
            {
                const auto itemSize = item.second->GetSize();
                switch(item.second->GetHorizontalSizePolicy())
                {
                    case Fixed:
                        width += itemSize.x;
                        break;
                    default:
                        expandThese.push_back(item.second);
                        expandedChildren++;
                        break;
                }
                switch(item.second->GetVerticalSizePolicy())
                {
                    case Expand:
                        item.second->SetSize({itemSize.x, m_size.y});
                        break;
                    case Clamp:
                        if(itemSize.y > m_size.y)
                            item.second->SetSize({itemSize.x, m_size.y});
                        break;
                    default:
                        break;
                }
            }

            if(expandedChildren == 0)
                return;

            expandSize = Util::Min<float>(((m_size.x - width) / expandedChildren) - (m_padding.x / 2), 0);
            if(expandSize < 1)
                return;

            for(auto i : expandThese)
                i->SetSize({expandSize, i->GetSize().y});
        }

        void HorizontalLayout::AlignChildren()
        {
            float offset = 0, width = 0, width2;
            for(auto& item : m_children)
                width += item.second->GetSize().x;
            width += m_children.size() * 5.0f;
            width2 = width / 2;

            switch(m_vAlignPolicy)
            {
                case Far:
                    offset = m_size.x - m_children.at(m_children.size() - 1).second->GetSize().x;
                    break;
                case Center:
                    offset = (m_size.x / 2) - width2;
                    break;
                default: break;
            }

            for(auto item = m_children.rbegin(); item != m_children.rend(); ++item)
            {
                auto* i = item->second;
                auto next = Util::Clamp(item+1, m_children.rbegin(), m_children.rend()-1);
                auto itemSize = i->GetSize();
                switch(m_hAlignPolicy)
                {
                    case Far:
                        i->SetPosition({m_pos.x + offset, m_pos.y + m_size.y - itemSize.y});
                        break;
                    case Center:
                        i->SetPosition({m_pos.x + offset, m_pos.y + ((m_size.y - itemSize.y) / 2)});
                        break;
                    default:
                        i->SetPosition({m_pos.x + offset, m_pos.y});
                        break;
                }
                if(m_vAlignPolicy == Far)
                    offset -= next->second->GetSize().x + m_padding.x;
                else
                    offset += i->GetSize().x + m_padding.x;
            }
        }
    }
}