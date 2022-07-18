#include <TML/Interface/Layouts/HorizontalLayout.h>

namespace tml::Interface
{
    HorizontalLayout::HorizontalLayout() noexcept
    {

    }

    HorizontalLayout::HorizontalLayout(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept
    {
        SetPosition(x,y);
        SetSize(w,h);
    }

    HorizontalLayout::HorizontalLayout(const std::vector<BaseComponent*>& components, int32_t x, int32_t y, uint32_t h) noexcept
    : HorizontalLayout(x, y, 0, h)
    {
        m_hSizePolicy = SizePolicy::Expand;
        m_vSizePolicy = SizePolicy::Fixed;

        if(h == 0)
        {
            int height = 0;

            for(auto* i : components)
            {
                AddChild(i);
                height = static_cast<int>(Math::Max<float>(height, i->GetSize().y));
            }

            SetSize(static_cast<int>(GetSize().x), height);
        }
        else
        {
            for(auto* i : components)
            {
                AddChild(i);
            }
        }

        m_originalSize = m_size;
    }

    bool HorizontalLayout::SetCenterVertically(bool value) noexcept
    {
        return m_verticallyCenter = value;
    }

    void HorizontalLayout::ScaleChildren() noexcept
    {
        std::vector<BaseComponent*> expandThese;
        std::vector<BaseComponent*> clampThese;
        float fixedSize = 0, clampSize = 0;

        for(auto& item : m_children)
        {
            const auto itemSize = item->GetSize();
            const auto originalSize = item->GetOriginalSize();
            switch(item->GetHorizontalSizePolicy())
            {
                case SizePolicy::Fixed:
                    fixedSize += itemSize.x + m_padding.x;
                    break;
                case SizePolicy::Clamp:
                    clampSize += itemSize.x + m_padding.x;
                    clampThese.push_back(item.get());
                    break;
                default:
                    expandThese.push_back(item.get());
                    break;
            }
            switch(item->GetVerticalSizePolicy())
            {
                case SizePolicy::Expand:
                    item->SetSize({itemSize.x, m_size.y});
                    break;
                case SizePolicy::Clamp:
                    if(itemSize.y > m_size.y)
                        item->SetSize({itemSize.x, m_size.y});
                    else if(itemSize.y < originalSize.y)
                        item->SetSize(itemSize.x, Math::Min<int32_t>(originalSize.y, m_size.y));
                    break;
                default:
                    break;
            }
        }
        float widthMinusFixedWidth = Math::Max<float>(m_size.x - fixedSize, 0);
        float expandSize = widthMinusFixedWidth; /// How much size there is to expand children.

        if(!clampThese.empty())
        {
            auto multiplier = Math::Max<float>(widthMinusFixedWidth / clampSize, 0);
            for(auto i : clampThese)
            {
                auto iSize = i->GetSize();
                auto iOldSize = i->GetOriginalSize();
                auto scale = Math::Clamp<float>(multiplier, 0, iOldSize.x / float(iSize.x));
                auto iNewSize = Vector2f(iSize.x * scale, iSize.y);
                i->SetSize(iNewSize);

                expandSize -= iNewSize.x;
            }
        }

        auto expandedChildren = expandThese.size();
        if(expandedChildren != 0)
        {
            auto size = expandSize / float(expandedChildren) - (m_padding.x / (expandedChildren / 2.f));

            for(auto i : expandThese)
                i->SetSize(Vector2i(size, i->GetSize().y));
        }
    }

    void HorizontalLayout::AlignChildren() noexcept
    {
        float offset = 0;

        for(auto& i : m_children)
        {
            float y = m_pos.y;

            if(m_verticallyCenter)
            {
                y = Math::Max(m_pos.y + (m_size.y - i->GetSize().y) / 2, m_pos.y);
            }

            i->SetPosition(m_pos.x + offset, y);
            offset += i->GetSize().x + m_padding.x;
        }
    }
}