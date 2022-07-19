#include <TML/Interface/Components/Listbox.h>

namespace tml::Interface
{
    Listbox::Listbox(uint32_t width, uint32_t height, int32_t x, int32_t y)
    : Component(x,y,width,height)
    {
        m_pos = Vector2i(x, y);
        m_size = Vector2i(width, height);
        m_scrollbar = new Scrollbar<ComponentAxis::Vertical>(x + width - 21, y+1, height-2);
        m_hSizePolicy = SizePolicy::Dynamic;
        m_vSizePolicy = SizePolicy::Dynamic;
        m_minimumSize = Vector2f(100, 20);

        AddChild(m_scrollbar);
        m_scrollbar->Disable();

        if(width)
        {
            m_minimumSize.x = static_cast<float>(width);
        }

        AddListener("Click", [&](Component* c, const Event& e)
        {
            if(m_state.MouseOver)
            {
                const Vector2i mousePos = {e.mouseButton.x, e.mouseButton.y};
                static auto PointInRect = [&](const Vector2i &tl, const Vector2i &br)
                {
                    return (mousePos.x <= br.x && mousePos.y <= br.y && mousePos.x >= tl.x && mousePos.y >= tl.y);
                };

                for(int i = 0; i < m_values.size(); i++)
                {
                    if(PointInRect(m_pos + Vector2i(0, i * 20), m_pos + Vector2i(m_size.x - 20, (i * 20) + 20)))
                    {
                        m_selectedIndex = i + m_scrollbar->GetValue();
                        break;
                    }
                }

                return true;
            }

            return false;
        });

        AddListener("MouseScroll", [&](Component* c, const Event& e)
        {
            if(m_state.MouseOver && m_scrollbar->Enabled())
            {
                if(e.mouseWheel.deltaY > 0.0)
                {
                    m_scrollbar->SetValue(m_scrollbar->GetValue() - 1);
                }
                else if(e.mouseWheel.deltaY < 0.0)
                {
                    m_scrollbar->SetValue(m_scrollbar->GetValue() + 1);
                }

                return true;
            }

            return false;
        });

        AddListener("Moved", [&](Component* c, const Event& e)
        {
            m_scrollbar->SetPosition(m_pos + Vector2i(m_size.x - 21, 1));
            return true;
        });

        AddListener("Resized", [&](Component* c, const Event& e)
        {
            m_size.y -= fmodf(m_size.y, 20);
            m_scrollbar->SetPosition(m_pos + Vector2i(m_size.x - 21, 0));
            m_scrollbar->SetSize({m_scrollbar->GetSize().x, m_size.y - 2});

            if(const auto overflow = GetOverFlow())
            {
                m_scrollbar->Enable();
                m_scrollbar->SetRange(0, overflow);
            }
            else
            {
                m_scrollbar->Disable();
            }

            return true;
        });

        AddListener("Drawn", [&](Component* c, const Event& e)
        {
            if(m_state.MouseOver)
            {
                m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress + e.update.delta * s_animationSpeed, 0, 1);
            }
            else
            {
                m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress - e.update.delta * s_animationSpeed, 0, 1);
            }

            if(m_state.Focused)
            {
                m_borderAnimationProgress = 1;
            }

            m_borderColor = Math::Lerp(m_sColor, m_activeColor, m_borderAnimationProgress);
            return true;
        });
    }

    void Listbox::AddValue(String value)
    {
        m_values.push_back(std::move(value));

        if(const auto overflow = GetOverFlow())
        {
            m_scrollbar->Enable();
            m_scrollbar->SetRange(0, overflow);
        }
        else
        {
            m_scrollbar->Disable();
        }
    }

    void Listbox::SetValue(uint32_t index, String value)
    {
        if(index > m_values.size() - 1)
        {
            return;
        }
        else
        {
            m_values.at(index) = std::move(value);
        }
    }

    String Listbox::GetValue(uint32_t index)
    {
        if(index >= m_values.size())
        {
            return {};
        }
        else
        {
            return m_values.at(index);
        }
    }

    String Listbox::GetSelectedValue() const
    {
        if(m_selectedIndex > -1 && m_selectedIndex < m_values.size())
        {
            return m_values.at(m_selectedIndex);
        }

        return "";
    }

    int32_t Listbox::GetSelectedIndex() const
    {
        return m_selectedIndex;
    }

    uint32_t Listbox::GetElementsCount() const
    {
        return m_values.size();
    }

    bool Listbox::ContainsValue(const std::string &value) const
    {
        for(auto &i : m_values)
        {
            if(i == value)
            {
                return true;
            }
        }

        return false;
    }

    void Listbox::RemoveValue(uint32_t index)
    {
        if(index >= 0 && index < m_values.size())
        {
            m_values.erase(m_values.begin() + index);
            m_selectedIndex = -1;

            if(const auto overflow = GetOverFlow())
            {
                m_scrollbar->Enable();
                m_scrollbar->SetRange(0, overflow);
            }
            else
            {
                m_scrollbar->Disable();
            }
        }
    }

    void Listbox::Clear()
    {
        m_values.clear();
    }

    int32_t Listbox::GetOverFlow() const noexcept
    {
        const float valuesSize = (m_values.size() * 20.f);
        return Math::Max<int32_t>((valuesSize - m_size.y) / 20, 0);
    }

    void Listbox::pDraw(RenderTarget& target) noexcept
    {
        target.DrawRect(m_pos, m_size, m_borderColor);
        target.DrawRect(m_pos + Vector2f(1, 1), m_size - Vector2f(2, 2), m_pColor);

        target.SetBounds(m_pos, m_size);
        target.DrawRect(m_pos + Vector2f(1, (m_selectedIndex - m_scrollbar->GetValue()) * 20 + 1), Vector2f(m_size.x-2, 20.f-2), m_activeColor);

        for(int i = 0; i < m_values.size(); i++)
        {
            const auto yPos = m_pos + Vector2f(0, i * 20 - (m_scrollbar->GetValue() * 20));

            if(yPos.y + 20 > m_size.y + m_pos.y)
            {
                break;
            }
            if(yPos.y + 20 > m_pos.y)
            {
                target.DrawText(m_values.at(i), m_pos + Vector2i(5, i * 20 - (m_scrollbar->GetValue() * 20)), 20, m_textColor);
            }
        }

        target.ResetBounds();
    }
}