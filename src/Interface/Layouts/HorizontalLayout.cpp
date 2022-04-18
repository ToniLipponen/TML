#include <TML/Interface/Layouts/HorizontalLayout.h>
#include <iostream>
namespace tml
{
    namespace Interface
    {
        HorizontalLayout::HorizontalLayout()
        : HorizontalLayout(0,0,0,0)
        {

        }

        HorizontalLayout::HorizontalLayout(int32_t x, int32_t y, uint32_t w, uint32_t h)
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

        void HorizontalLayout::pDraw(Renderer& renderer)
        {
            renderer.DrawRect(m_pos, m_size, m_pColor);
        }

        void HorizontalLayout::ScaleChildren()
        {
            std::vector<BaseComponent*> expandThese;
            float fixedSize = 0;
            for(auto* item : m_children)
            {
                const auto itemSize = item->GetSize();
                const auto originalSize = item->GetOriginalSize();
                switch(item->GetHorizontalSizePolicy())
                {
                    case Fixed:
                        fixedSize += itemSize.x + m_padding.x;
                        break;
                    case Expand:
                        expandThese.push_back(item);
                        break;
                    default:
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
                            item->SetSize({itemSize.x, Math::Min<int32_t>(originalSize.y, m_size.y)});
                        break;
                    default:
                        break;
                }
            }
            float widthMinusFixedWidth = Math::Max<float>(m_size.x - fixedSize, 0);
            float expandSize = widthMinusFixedWidth; /// How much size there is to expand children.

            auto expandedChildren = expandThese.size();
            if(expandedChildren != 0)
            {
                auto size = expandSize / float(expandedChildren) - (m_padding.x / (expandedChildren / 2.f));

                for(auto i : expandThese)
                    i->SetSize(Vector2i(size, i->GetSize().y));
            }
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