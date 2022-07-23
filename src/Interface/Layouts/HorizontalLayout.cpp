#include <TML/Interface/Layouts/HorizontalLayout.h>

namespace tml::Interface
{
    HorizontalLayout::HorizontalLayout() noexcept = default;

    HorizontalLayout::HorizontalLayout(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept
    {
        SetPosition(x,y);
        SetSize(w,h);
        m_verticallyCenter = false;
    }

    HorizontalLayout::HorizontalLayout(const std::vector<Component*>& components, int32_t x, int32_t y, uint32_t h) noexcept
    : HorizontalLayout(x, y, 0, h)
    {
        SetSizePolicy(SizePolicy::Dynamic, SizePolicy::Fixed);
        m_margin = {0, 0};
        m_verticallyCenter = true;

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

    bool HorizontalLayout::GetCenterVertically() const noexcept
    {
        return m_verticallyCenter;
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
                i->SetSize(i->GetSize().x, m_size.y - m_margin.y * 2);
            }
        }

        const int64_t children = m_children.size();
        float size = m_size.x - m_margin.x * 2 -  fixedSize - m_padding.x * Math::Max<int64_t>(children - 1, 1);

        if(!scaledChildren.empty())
        {
            struct Bucket
            {
                float min, max, size;
            };

            std::vector<Bucket> buckets;
            buckets.reserve(scaledChildren.size());

            for(auto* i : scaledChildren)
            {
                Bucket bucket{};
                bucket.min = i->GetMinimumSize().x;
                bucket.max = i->GetMaximumSize().x;
                bucket.size = bucket.min;
                size -= bucket.min;
                buckets.push_back(bucket);
            }

            while(size > 0)
            {
                bool incremented = false;

                for(auto& bucket : buckets)
                {
                    if(bucket.size < bucket.max)
                    {
                        bucket.size += 1;
                        size -= 1;
                        incremented = true;
                    }
                }

                if(!incremented)
                {
                    break;
                }
            }

            for(int i = 0; i < scaledChildren.size(); i++)
            {
                scaledChildren.at(i)->SetSize(buckets.at(i).size, scaledChildren.at(i)->GetSize().y);
            }
        }
    }

    void HorizontalLayout::AlignChildren() noexcept
    {
        float offset = m_margin.x;

        for(auto& i : m_children)
        {
            float y = m_pos.y + m_margin.y;

            if(m_verticallyCenter)
            {
                y = Math::Max(m_pos.y + (m_size.y - i->GetSize().y) / 2, m_pos.y);
            }

            i->SetPosition(m_pos.x + offset, y);
            offset += i->GetSize().x + m_padding.x;
        }
    }
}