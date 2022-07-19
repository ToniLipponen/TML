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

    HorizontalLayout::HorizontalLayout(const std::vector<Component*>& components, int32_t x, int32_t y, uint32_t h) noexcept
    : HorizontalLayout(x, y, 0, h)
    {
        m_hSizePolicy = SizePolicy::Dynamic;
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
    }

    bool HorizontalLayout::SetCenterVertically(bool value) noexcept
    {
        return m_verticallyCenter = value;
    }

    void HorizontalLayout::ScaleChildren() noexcept
    {
        std::vector<Component*> scaledChildren;
        float fixedSize = 0;

        for(auto& i : m_children)
        {
            if(i->GetHorizontalSizePolicy() == SizePolicy::Fixed)
            {
                fixedSize += i->GetSize().x;
            }
            else
            {
                scaledChildren.push_back(i.get());
            }

            if(i->GetVerticalSizePolicy() == SizePolicy::Dynamic)
            {
                i->SetSize(i->GetSize().x, m_size.y);
            }
        }

        const int64_t children = m_children.size();
        float size = m_size.x - fixedSize - m_padding.x * Math::Max<int64_t>(children - 1, 1);

        if(!scaledChildren.empty())
        {
            for(int64_t i = scaledChildren.size() - 1; i >= 0; i--)
            {
                auto* child = scaledChildren.at(i);

                const auto maxWidth = child->GetMaximumSize().x;
                const auto minWidth = child->GetMinimumSize().x;
                const auto itemSize = size / scaledChildren.size();
                const auto height = child->GetSize().y;
                const auto width = Math::Clamp(itemSize, minWidth, maxWidth);
                child->SetSize(width, height);
                size -= width;
                scaledChildren.pop_back();
            }
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