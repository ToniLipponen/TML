#include <TML/Interface/Layouts/VerticalLayout.h>
#include <TML/Utilities/Utilities.h>

namespace tml
{
    namespace Interface
    {
        VerticalLayout::VerticalLayout(i32 x, i32 y, ui32 w, ui32 h)
        : BaseComponent(x,y,w,h)
        {
            m_hSizePolicy = Expand;
            m_vSizePolicy = Expand;
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

        void VerticalLayout::ScaleChildren()
        {
            std::vector<BaseComponent*> expandThese, clampThese;
            float height = 0, expandSize, clampHeight = 0;
            for(auto& item : m_children)
            {
                const auto itemSize = item->GetSize();
                switch(item->GetVerticalSizePolicy())
                {
                    case Fixed:
                        height += itemSize.y + m_padding.y;
                        break;
                    case Clamp:
                        clampHeight += itemSize.y + m_padding.y;
                        clampThese.push_back(item);
                        break;
                    default: /// Expand
                        expandThese.push_back(item);
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

            const auto expandedChildren = expandThese.size();
            const auto clampedChildren = clampThese.size();

            expandSize = Math::Max<float>(((m_size.y - height) / expandedChildren) - (m_padding.y / 2) - 1, 0);

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

            for(auto item = m_children.begin(); item != m_children.end(); ++item)
            {
                auto* i = *item;
                auto* next = *Math::Clamp(item+1, m_children.begin(), m_children.end()-1);
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
    }
}