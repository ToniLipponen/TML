#include <TML/Interface/Components/Listbox.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    namespace Interface
    {
        Listbox::Listbox(i32 x, i32 y, ui32 width, ui32 height)
        {
            m_pos = Vector2i(x, y);
            m_size = Vector2i(width, height);
            m_scrollbar = new Scrollbar<Vertical>(x + width - 21, y+1, height-2);
            m_scrollbar->Disable();
            AddChild(m_scrollbar);
        }

        void Listbox::AddValue(std::string value)
        {
            m_values.push_back(std::move(value));
            const auto overflow = GetOverFlow();
            if(overflow > 0)
            {
                m_scrollbar->Enable();
                m_scrollbar->SetRange(0, overflow);
            }
        }

        void Listbox::SetValue(ui32 index, std::string value)
        {
            if(index > m_values.size() - 1)
                return;
            else
                m_values.at(index) = std::move(value);
        }

        std::string Listbox::GetValue(ui32 index)
        {
            if(index >= m_values.size())
                return "";
            else
                return m_values.at(index);
        }

        std::string Listbox::GetSelectedValue() const
        {
            if(m_selectedIndex > -1 && m_selectedIndex < m_values.size())
            {
                return m_values.at(m_selectedIndex);
            }
            return "";
        }

        tml::i32 Listbox::GetSelectedIndex() const
        {
            return m_selectedIndex;
        }

        tml::ui32 Listbox::GetElementsCount() const
        {
            return m_values.size();
        }

        bool Listbox::ContainsValue(const std::string &value) const
        {
            for(auto &i: m_values) {
                if(i == value)
                    return true;
            }
            return false;
        }

        void Listbox::RemoveValue(ui32 index)
        {
            if(index >= 0 && index < m_values.size())
            {
                m_values.erase(m_values.begin() + index);
                m_selectedIndex = -1;

                const auto overflow = GetOverFlow();
                if(overflow == 0)
                    m_scrollbar->Disable();
                else
                    m_scrollbar->SetRange(0, overflow);
            }
        }

        void Listbox::Clear()
        {
            m_values.clear();
        }

        ui32 Listbox::GetOverFlow() const noexcept
        {
            const float valuesSize = (m_values.size() * 20.f);
            if(valuesSize > m_size.y)
                return Math::Min<ui32>((valuesSize - m_size.y) / 20, 1);

            return 0;
        }

        void Listbox::OnMoved()
        {
            m_scrollbar->SetPosition(m_pos + Vector2i(m_size.x - 21, 1));
        }

        void Listbox::OnResized()
        {
            m_scrollbar->SetPosition(m_pos + Vector2i(m_size.x - 21, 1));
            m_scrollbar->SetSize({m_scrollbar->GetSize().x, m_size.y - 2});

            const auto overflow = GetOverFlow();
            if(overflow > 0)
            {
                m_scrollbar->Enable();
                m_scrollbar->SetRange(0, overflow);
            }
        }

        void Listbox::OnMouseClick(const Vector2i &mousePos)
        {
            auto PointInRect = [&](const Vector2i &tl, const Vector2i &br)
            {
                return (mousePos < br && mousePos > tl);
            };
            for(int i = 0; i < m_values.size(); i++)
            {
                if(PointInRect(m_pos + Vector2i(0, i * 20), m_pos + Vector2i(m_size.x - 20, (i * 20) + 20)))
                {
                    m_selectedIndex = i + m_scrollbar->GetValue();
                    break;
                }
            }
        }

        void Listbox::OnUpdate(double dt)
        {
            const auto value = Mouse::GetScrollValue();
            if(m_eventStatus.MouseOver && m_scrollbar->Enabled())
            {
                if(value > 0.0)
                    m_scrollbar->SetValue(m_scrollbar->GetValue() - 1);
                else if(value < 0.0)
                    m_scrollbar->SetValue(m_scrollbar->GetValue() + 1);
            }
        }

        void Listbox::Draw()
        {
            Renderer::DrawRect(m_pos, m_size, m_pColor);

            const Vector2i pos = m_pos + Vector2i(1, 1);
            const Vector2i size = m_size - Vector2i(2, 2);

            if(m_selectedIndex >= 0 && m_selectedIndex < m_values.size() && Math::InRange<float>(0, m_selectedIndex * 20 - (m_scrollbar->GetValue() * 20), m_size.y - 20))
            {
                Renderer::DrawRect(m_pos + Vector2i(0, m_selectedIndex * 20 - (m_scrollbar->GetValue() * 20)), Vector2f(m_size.x, 20.f), m_activeColor);
            }

            Renderer::SetBounds(m_pos, m_size);
            for(int i = 0; i < m_values.size(); i++)
            {
                Renderer::DrawText(m_values.at(i), m_pos + Vector2i(5, i * 20 - (m_scrollbar->GetValue() * 20)), 20, BLACK);
            }
            Renderer::ResetBounds();

            if(m_state.Focused)
                Renderer::DrawGrid(pos, size, 1, 1, m_activeColor, 1);
            else
                Renderer::DrawGrid(pos, size, 1, 1, m_sColor, 1);
        }
    }
}