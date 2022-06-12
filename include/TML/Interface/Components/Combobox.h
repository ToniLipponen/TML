#pragma once
#include "../BaseComponent.h"
#include "Listbox.h"
#include "TextInput.h"
#include <string>

namespace tml::Interface
{
    class TML_API Combobox : public BaseComponent
    {
    public:
        Combobox(int32_t x, int32_t y, uint32_t width, uint32_t height = 20);
        void AddValue(String value);
        void SetValue(uint32_t index, String value);
        void SetListHeight(int height);
        int32_t GetSelectedIndex() const;
        String GetValue(uint32_t index);
        String GetSelectedValue() const;
        void Clear(); //!< Deletes every item in the list.

    private:
        void pDraw(RenderTarget& target) noexcept override;
        Listbox* m_listComponent;
        TextInput* m_textInput;
        Color m_borderColor, m_bodyColor;
        double m_borderAnimationProgress = 0;
        float m_roundness = 0;
    };
}