#pragma once
#include <TML/Interface/Component.h>
#include <TML/Interface/Components/Button.h>
#include <TML/Interface/Components/Label.h>
#include <TML/Interface/Layouts/HorizontalLayout.h>

namespace tml::Interface
{
    class OptionSelector : public Component
    {
    public:
        OptionSelector() noexcept;
        OptionSelector(const std::vector<std::string>& options) noexcept;

        [[nodiscard]]
        uint64_t GetSelectedIndex() const noexcept;

        [[nodiscard]]
        std::string GetSelectedValue() const noexcept;

        bool SetSelectedValue(uint64_t index) noexcept;

        bool SetSelectedValue(const std::string& value) noexcept;

        std::string AddOption(const std::string& option) noexcept;

        bool RemoveOption(const std::string& option) noexcept;

        bool RemoveOption(uint64_t index) noexcept;

    protected:
        void pDraw(RenderTarget& target) noexcept override;

        std::vector<std::string> m_options;
        uint64_t m_selectedIndex = 0;
        HorizontalLayout* m_layout;
        Label* m_selectedLabel;
        Button* m_incement;
        Button* m_decrement;
    };
}