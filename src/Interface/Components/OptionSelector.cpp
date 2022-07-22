#include <TML/Interface/Components/OptionSelector.h>
#include "TML/Interface/Components/Spacer.h"

namespace tml::Interface
{
    OptionSelector::OptionSelector() noexcept
    : Component(0,0,0,0)
    {
        m_vSizePolicy = SizePolicy::Dynamic;
        m_hSizePolicy = SizePolicy::Dynamic;
        m_minimumSize = {200, 0};
        m_maximumSize = {10000, 30};
    }

    OptionSelector::OptionSelector(const std::vector<std::string>& options) noexcept
    : m_options(options)
    {
        m_vSizePolicy = SizePolicy::Dynamic;
        m_hSizePolicy = SizePolicy::Dynamic;
        m_minimumSize = {0, 20};
        m_maximumSize = {10000, 30};

        m_layout = new HorizontalLayout({
            m_decrement = new Button("<"),
            new Spacer,
            m_selectedLabel = new Label(options.at(0)),
            new Spacer,
            m_incement = new Button(">"),
        });

        m_layout->SetCenterVertically(true);

        AddChild(m_layout);

        AddListener("Moved", [&](Component* c, const Event& e)
        {
            m_layout->SetPosition(c->GetPosition());
            return true;
        });

        AddListener("Resized", [&](Component* c, const Event& e)
        {
            m_layout->SetSize(c->GetSize());
            return true;
        });

        m_incement->AddListener("Click", [&](Component* c, const Event& e)
        {
            m_selectedIndex = Math::Clamp<uint64_t>(m_selectedIndex + 1, 0, m_options.size() - 1);
            m_selectedLabel->SetValue(GetSelectedValue());
            return true;
        });

        m_decrement->AddListener("Click", [&](Component* c, const Event& e)
        {
            m_selectedIndex = Math::Clamp<int64_t>(static_cast<int64_t>(m_selectedIndex) - 1,
                                                   0,
                                                   static_cast<int64_t>(m_options.size()) - 1);

            m_selectedLabel->SetValue(GetSelectedValue());
            return true;
        });
    }

    uint64_t OptionSelector::GetSelectedIndex() const noexcept
    {
        return m_selectedIndex;
    }

    std::string OptionSelector::GetSelectedValue() const noexcept
    {
        if(m_selectedIndex < m_options.size())
        {
            return m_options.at(m_selectedIndex);
        }

        return "";
    }

    bool OptionSelector::SetSelectedValue(uint64_t index) noexcept
    {
        if(index < m_options.size())
        {
            m_selectedIndex = index;
            return true;
        }

        return false;
    }

    bool OptionSelector::SetSelectedValue(const std::string& value) noexcept
    {
        for(size_t i = 0; i < m_options.size(); i++)
        {
            if(m_options.at(i) == value)
            {
                m_selectedIndex = i;
                return true;
            }
        }

        return false;
    }

    std::string OptionSelector::AddOption(const std::string& option) noexcept
    {
        m_options.push_back(option);
        return option;
    }

    bool OptionSelector::RemoveOption(const std::string& option) noexcept
    {
        for(size_t i = 0; i < m_options.size(); i++)
        {
            if(m_options.at(i) == option)
            {
                m_options.erase(m_options.begin() + i);
                return true;
            }
        }

        return false;
    }

    bool OptionSelector::RemoveOption(uint64_t index) noexcept
    {
        return false;
    }

    void OptionSelector::pDraw(RenderTarget& target) noexcept
    {
//        target.DrawRect(m_pos, m_size, m_pColor);
    }
}