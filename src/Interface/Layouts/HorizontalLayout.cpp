#include <TML/Interface/Layouts/HorizontalLayout.h>
#include <TML/Utilities/Utilities.h>

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

        void HorizontalLayout::ScaleChildren()
        {
            std::vector<BaseComponent*> expandThese;
            ui32 expandedChildren = 0;
            float width = 0, expandSize;
            for(auto* item : m_children)
            {
                const auto itemSize = item->GetSize();
                const auto originalSize = item->GetOriginalSize();
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
                        else if(itemSize.y < originalSize.y)
                            item->SetSize({itemSize.x, Math::Min<i32>(originalSize.y, m_size.y)});
                        break;
                    default:
                        break;
                }
            }

            if(expandedChildren == 0)
                return;

            expandSize = Math::Max<float>(((m_size.x - width) / expandedChildren) - (m_padding.x * m_children.size() / 2), 0);

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

            for(auto item = m_children.begin(); item != m_children.end(); ++item)
            {
                auto* i = *item;
                auto* next = *Math::Clamp(item+1, m_children.begin(), m_children.end()-1);
                auto itemSize = i->GetSize();
                i->SetPosition({static_cast<int>(m_pos.x + offset), m_pos.y});
                offset += i->GetSize().x + m_padding.x;
            }
        }
    }
}