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

            AddListener("ChildAdded", [&](BaseComponent* c, Event& e)
            {
                ScaleChildren();
                AlignChildren();
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
                const auto originalSize = item->GetOriginalSize();
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
                        else if(itemSize.x < originalSize.x)
                            item->SetSize({Math::Min<i32>(originalSize.x, m_size.x), itemSize.y});
                        break;
                    default:
                        break;
                }
            }

            const auto expandedChildren = expandThese.size();

            expandSize = Math::Max<float>(((m_size.y - height) / expandedChildren) - (m_padding.y * m_children.size() / 2), 0);

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

            for(auto item = m_children.begin(); item != m_children.end(); ++item)
            {
                auto* i = *item;
                auto* next = *Math::Clamp(item+1, m_children.begin(), m_children.end()-1);
                auto itemSize = i->GetSize();
                i->SetPosition({m_pos.x, static_cast<int>(m_pos.y + offset)});
                offset += i->GetSize().y + m_padding.y;
            }
        }
    }
}