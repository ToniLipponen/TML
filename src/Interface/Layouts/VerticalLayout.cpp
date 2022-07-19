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
            for(int64_t i = scaledChildren.size() - 1; i >= 0; i--)
            {
                auto* child = scaledChildren.at(i);

                const auto maxHeight = child->GetMaximumSize().y;
                const auto minHeight = child->GetMinimumSize().y;
                const auto itemSize = size / scaledChildren.size();
                const auto height = Math::Clamp(itemSize, minHeight, maxHeight);
                const auto width = child->GetSize().x;
                child->SetSize(width, height);
                size -= width + m_padding.y;
                scaledChildren.pop_back();
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