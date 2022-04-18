#include <TML/Interface/Layouts/VerticalLayout.h>

namespace tml
{
    namespace Interface
    {
        VerticalLayout::VerticalLayout(int32_t x, int32_t y, uint32_t w, uint32_t h)
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
            float height = 0;
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
                            item->SetSize({Math::Min<int32_t>(originalSize.x, m_size.x), itemSize.y});
                        break;
                    default:
                        break;
                }
            }

            float heightMinusFixedHeight = Math::Max<float>(m_size.y - height, 0);
            float expandSize = heightMinusFixedHeight;

            if(!clampThese.empty())
            {
                auto multiplier = Math::Max<float>(heightMinusFixedHeight / height, 0);
                for(auto i : clampThese)
                {
                    auto iSize = i->GetSize();
                    auto iOldSize = i->GetOriginalSize();
                    auto scale = Math::Clamp<float>(multiplier, 0, iOldSize.y / float(iSize.y));
                    auto iNewSize = Vector2i(iSize.x, iSize.y * scale);
                    i->SetSize(iNewSize);

                    expandSize -= iNewSize.y;
                }
            }

            const auto expandedChildren = expandThese.size();
            if(expandedChildren != 0)
            {
                auto size = Math::Max<float>((expandSize / expandedChildren) - (m_padding.y * m_children.size() / 2), 0);

                for(auto i : expandThese)
                    i->SetSize(Vector2i(i->GetSize().x, size));
            }
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