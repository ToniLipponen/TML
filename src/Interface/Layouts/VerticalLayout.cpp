#include <TML/Interface/Layouts/VerticalLayout.h>
#include <TML/Utilities/Utilities.h>

namespace tml
{
    namespace Interface
    {
        VerticalLayout::VerticalLayout(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(w,h);
            m_hSizePolicy = Expand;
            m_vSizePolicy = Expand;
        }

        void VerticalLayout::OnUpdate(double dt)
        {
            static ui64 oldChildrenSize = 0;
            if(m_children.size() != oldChildrenSize)
            {
                ScaleChildren();
                AlignChildren();
                oldChildrenSize = m_children.size();
            }
        }

        void VerticalLayout::ScaleChildren()
        {
            std::vector<BaseComponent*> expandThese;
            ui32 expandedChildren = 0;
            float height = 0, expandSize;
            for(auto& item : m_children)
            {
                const auto itemSize = item->GetSize();
                switch(item->GetVerticalSizePolicy())
                {
                    case Fixed:
                        height += itemSize.y;
                        break;
                    default:
                        expandThese.push_back(item);
                        expandedChildren++;
                        break;
                }
                switch(item->GetHorizontalSizePolicy())
                {
                    case Expand:
                        item->SetSize({m_size.x, itemSize.y});
                        break;
                    case Clamp:
                        if(itemSize.x > m_size.x)
                            item->SetSize({m_size.x, itemSize.y});
                        break;
                    default:
                        break;
                }
            }

            if(expandedChildren == 0)
                return;

            expandSize = Math::Min<float>(((m_size.y - height) / expandedChildren) - (m_padding.y / 2) - 1, 0);
            if(expandSize < 1)
                return;

            for(auto i : expandThese)
                i->SetSize({i->GetSize().x, static_cast<int>(expandSize)});
        }

        void VerticalLayout::AlignChildren()
        {
            float offset = 0, height = 0, height2;
            for(auto& item : m_children)
                height += item->GetSize().y;
            height += m_children.size() * 5.0f;
            height2 = height / 2;

            switch(m_vAlignPolicy)
            {
                case Far:
                    offset = m_size.y - m_children.at(m_children.size() - 1)->GetSize().y;
                    break;
                case Center:
                    offset = (m_size.y / 2) - height2;
                    break;
                default: break;
            }

            for(auto item = m_children.rbegin(); item != m_children.rend(); ++item)
            {
                auto* i = *item;
                auto* next = *Math::Clamp(item+1, m_children.rbegin(), m_children.rend()-1);
                auto itemSize = i->GetSize();
                switch(m_hAlignPolicy)
                {
                    case Far:
                        i->SetPosition({m_pos.x + m_size.x - itemSize.x, static_cast<int>(m_pos.y + offset)});
                        break;
                    case Center:
                        i->SetPosition({m_pos.x + ((m_size.x - itemSize.x) / 2), static_cast<int>(m_pos.y + offset)});
                        break;
                    default:
                        i->SetPosition({m_pos.x, static_cast<int>(m_pos.y + offset)});
                        break;
                }
                if(m_vAlignPolicy == Far)
                    offset -= next->GetSize().y + m_padding.y;
                else
                    offset += i->GetSize().y + m_padding.y;
            }
        }

        void VerticalLayout::OnResized()
        {
            ScaleChildren();
            AlignChildren();
        }

        void VerticalLayout::OnMoved()
        {
            ScaleChildren();
            AlignChildren();
        }
    }
}