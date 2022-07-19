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
        SetSizePolicy(SizePolicy::Dynamic, SizePolicy::Fixed);

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
        std::vector<Component*> scaledChildren;
        float fixedSize = 0;

        for(auto& i : m_children)
        {
            if(i->GetVerticalSizePolicy() == SizePolicy::Fixed)
            {
                fixedSize += i->GetSize().y;
            }
            else
            {
                scaledChildren.push_back(i.get());
            }

            if(i->GetHorizontalSizePolicy() == SizePolicy::Dynamic)
            {
                i->SetSize(m_size.x, i->GetSize().y);
            }
        }

        const int64_t children = m_children.size();
        float size = m_size.y - fixedSize - m_padding.y * Math::Max<int64_t>(children - 1, 1);

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
                bucket.min = i->GetMinimumSize().y;
                bucket.max = i->GetMaximumSize().y;
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
                scaledChildren.at(i)->SetSize(scaledChildren.at(i)->GetSize().x, buckets.at(i).size);
            }
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