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
            /*
             * Might not be wise to call these on every update.
             * But currently there isn't OnResize or OnMove callbacks.
             * TODO: Create those aforementioned callbacks.
             */
            ScaleChildren();
            AlignChildren();
        }

        void VerticalLayout::ScaleChildren()
        {
            std::vector<BaseComponent*> expandThese;
            ui32 expandedChildren = 0;
            float height = 0, expandSize;
            for(auto& item : m_children)
            {
                const auto itemSize = item.second->GetSize();
                switch(item.second->GetVerticalSizePolicy())
                {
                    case Expand:
                        expandThese.push_back(item.second);
                        expandedChildren++;
                        break;
                    default:
                        height += itemSize.y;
                        break;
                }
                switch(item.second->GetHorizontalSizePolicy())
                {
                    case Expand:
                        item.second->SetSize({m_size.x, itemSize.y});
                        break;
                    case Clamp:
                        if(itemSize.x > m_size.x)
                            item.second->SetSize({m_size.x, itemSize.y});
                        break;
                    default:
                        break;
                }
            }
            if(expandedChildren == 0)
                return;

            expandSize = Util::Min<float>(((m_size.y - height) / expandedChildren), 0);
            if(expandSize < 1)
                return;
            for(auto i : expandThese)
                i->SetSize({i->GetSize().x, expandSize});
        }

        void VerticalLayout::AlignChildren()
        {
            float offset = 0, height = 0, height2;
            for(auto& item : m_children)
                height += item.second->GetSize().y;
            height += m_children.size() * 5.0f;
            height2 = height / 2;

            switch(m_vAlignPolicy)
            {
                case Far:
                    offset = m_size.y - m_children.at(m_children.size() - 1).second->GetSize().y;
                    break;
                case Center:
                    offset = (m_size.y / 2) - height2;
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
                        i->SetPosition({m_pos.x + m_size.x - itemSize.x, m_pos.y + offset});
                        break;
                    case Center:
                        i->SetPosition({m_pos.x + ((m_size.x - itemSize.x) / 2), m_pos.y + offset});
                        break;
                    default:
                        i->SetPosition({m_pos.x, m_pos.y + offset});
                        break;
                }
                if(m_vAlignPolicy == Far)
                    offset -= next->second->GetSize().y + m_padding.y;
                else
                    offset += i->GetSize().y + m_padding.y;
            }
        }
    }
}