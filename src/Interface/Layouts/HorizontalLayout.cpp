#include <TML/Interface/Layouts/HorizontalLayout.h>
#include <TML/Utilities/Utilities.h>
#include <vector>

namespace tml
{
    namespace Interface
    {
        HorizontalLayout::HorizontalLayout(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(w,h);
            m_hSizePolicy = Expand;
            m_vSizePolicy = Fixed;
            AddListener("InterfaceUpdate", [&](BaseComponent* c, Event& e)
            {
                static ui64 oldChildrenSize = 0;
                if(m_children.size() != oldChildrenSize)
                {
                    ScaleChildren();
                    AlignChildren();
                    oldChildrenSize = m_children.size();
                }
            });

            AddListener("Resized", [&](BaseComponent* c, Event& e)
            {
                ScaleChildren();
                AlignChildren();
            });

            AddListener("Moved", [&](BaseComponent* c, Event& e)
            {
                ScaleChildren();
                AlignChildren();
            });
        }

        void HorizontalLayout::ScaleChildren()
        {
            std::vector<BaseComponent*> expandThese;
            ui32 expandedChildren = 0;
            float width = 0, expandSize;
            for(auto* item : m_children)
            {
                const auto itemSize = item->GetSize();
                switch(item->GetHorizontalSizePolicy())
                {
                    case Fixed:
                        width += itemSize.x;
                        break;
                    default:
                        expandThese.push_back(item);
                        expandedChildren++;
                        break;
                }
                switch(item->GetVerticalSizePolicy())
                {
                    case Expand:
                        item->SetSize({itemSize.x, m_size.y});
                        break;
                    case Clamp:
                        if(itemSize.y > m_size.y)
                            item->SetSize({itemSize.x, m_size.y});
                        break;
                    default:
                        break;
                }
            }

            if(expandedChildren == 0)
                return;

            expandSize = Math::Max<float>(((m_size.x - width) / expandedChildren) - (m_padding.x / 2), 0);

            for(auto i : expandThese)
                i->SetSize({static_cast<int>(expandSize), i->GetSize().y});
        }

        void HorizontalLayout::AlignChildren()
        {
            float offset = 0, width = 0, width2;
            for(auto* item : m_children)
                width += item->GetSize().x;
            width += m_children.size() * 5;
            width2 = width / 2;

            switch(m_vAlignPolicy)
            {
                case Far:
                    offset = m_size.x - m_children.at(m_children.size() - 1)->GetSize().x;
                    break;
                case Center:
                    offset = (m_size.x / 2) - width2;
                    break;
                default:
                    break;
            }

            for(auto item = m_children.begin(); item != m_children.end(); ++item)
            {
                auto* i = *item;
                auto* next = *Math::Clamp(item+1, m_children.begin(), m_children.end()-1);
                auto itemSize = i->GetSize();
                switch(m_hAlignPolicy)
                {
                    case Far:
                        i->SetPosition({static_cast<int>(m_pos.x + offset), m_pos.y + m_size.y - itemSize.y});
                        break;
                    case Center:
                        i->SetPosition({static_cast<int>(m_pos.x + offset), m_pos.y + ((m_size.y - itemSize.y) / 2)});
                        break;
                    default:
                        i->SetPosition({static_cast<int>(m_pos.x + offset), m_pos.y});
//                        Logger::InfoMessage("X: %d Y: %d", i->GetPosition().x, i->GetPosition().y);
                        break;
                }
                if(m_vAlignPolicy == Far)
                    offset -= next->GetSize().x + m_padding.x;
                else
                    offset += i->GetSize().x + m_padding.x;
            }
        }
    }
}