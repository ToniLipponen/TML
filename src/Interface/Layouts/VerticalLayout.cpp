#include <TML/Interface/Layouts/VerticalLayout.h>
#include <iostream>

namespace tml::Interface
{
    VerticalLayout::VerticalLayout() noexcept
    {

    }

    VerticalLayout::VerticalLayout(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept
    {
        SetPosition(x,y);
        SetSize(w,h);
    }

    VerticalLayout::VerticalLayout(const std::vector<Component*>& components, int32_t x, int32_t y, uint32_t w) noexcept
    : VerticalLayout(x,y,w,0)
    {
        SetSizePolicy(SizePolicy::Expand, SizePolicy::Fixed);

        for(auto* i : components)
        {
            AddChild(i);
        }

        TML_GUI_ON_EVENT("Attached",
            float width = m_size.x;
            float height = 0;

            for(auto& i : m_children)
            {
                auto iWidth = i->GetSize().x;

                if(iWidth > width)
                {
                    width = iWidth;
                }

                height += i->GetSize().y + m_padding.y;
            }

            SetSize(width, height);
            return false;
        );
    }

    void VerticalLayout::ScaleChildren() noexcept
    {
        std::vector<Component*> expandThese, clampThese;
        float height = 0;

        for(auto& item : m_children)
        {
            const auto itemSize = item->GetSize();
            const auto originalSize = item->GetOriginalSize();

            switch(item->GetVerticalSizePolicy())
            {
                case SizePolicy::Fixed:
                    height += itemSize.y + m_padding.y;
                    break;
                case SizePolicy::Clamp:
                    clampThese.push_back(item.get());
                    break;
                default: /// Expand
                    expandThese.push_back(item.get());
                    break;
            }

            switch(item->GetHorizontalSizePolicy())
            {
                case SizePolicy::Expand:
                {
                    item->SetSize({m_size.x, itemSize.y});
                } break;
                case SizePolicy::Clamp:
                {
                    if(itemSize.x > m_size.x)
                    {
                        item->SetSize({m_size.x, itemSize.y});
                    }
                    else if(itemSize.x < originalSize.x)
                    {
                        item->SetSize(Vector2i(Math::Min<int32_t>(originalSize.x, m_size.x), itemSize.y));
                    }
                } break;
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

    void VerticalLayout::AlignChildren() noexcept
    {
        float offset = 0;

        for(auto & i : m_children)
        {
            i->SetPosition(Vector2i(m_pos.x, static_cast<int>(m_pos.y + offset)));
            offset += i->GetSize().y + m_padding.y;
        }
    }
}