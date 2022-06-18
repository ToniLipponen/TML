#pragma once
#include "../BaseComponent.h"
#include <TML/Interface/Components/Scrollbar.h>
#include <vector>

namespace tml::Interface
{
    class TML_API Listbox : public BaseComponent
    {
    public:
        Listbox(uint32_t width, uint32_t height, int32_t x = 0, int32_t y = 0);
        void AddValue(String value);
        void SetValue(uint32_t index, String value);
        String GetValue(uint32_t index); //!< Returns an empty string if index is greater than list size - 1.
        String GetSelectedValue() const;
        int32_t GetSelectedIndex() const;
        uint32_t GetElementsCount() const;
        bool ContainsValue(const std::string& value) const;
        void RemoveValue(uint32_t index);
        void Clear();
        friend class Combobox;

    private:
        int32_t GetOverFlow() const noexcept; //!< Get count of items that do not fit int the visible area.

    protected:
        void pDraw(RenderTarget& target) noexcept override;
        std::vector<String> m_values;
        Scrollbar<ComponentAxis::Vertical>* m_scrollbar = nullptr;

        float m_scrollDist = 0.f;
        int32_t m_selectedIndex = -1;
        double m_borderAnimationProgress = 0;
        Color m_borderColor;
    };
}