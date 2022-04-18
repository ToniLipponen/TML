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

        void VerticalLayout::pDraw(Renderer& renderer)
        {
            renderer.DrawRect(m_pos, m_size, m_pColor);
        }

        void VerticalLayout::ScaleChildren()
        {
            std::vector<BaseComponent*> expandThese;
            float height = 0;
            for(auto* item : m_children)
            {
                const auto itemSize = item->GetSize();

                switch(item->GetVerticalSizePolicy())
                {
                    case Expand:
                        expandThese.push_back(item);
                        break;
                    default: /// Expand
                        height += itemSize.y + m_padding.y;
                        break;
                }
                switch(item->GetHorizontalSizePolicy())
                {
                    case Expand:
                        item->SetSize({m_size.x, itemSize.y});
                        break;
                    default:
                        break;
                }
            }

            float heightMinusFixedHeight = Math::Max<float>(m_size.y - height, 0);
            float expandSize = heightMinusFixedHeight;

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