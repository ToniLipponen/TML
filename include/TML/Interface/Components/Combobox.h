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
        Combobox(uint32_t width, uint32_t height = 20, int32_t x = 0, int32_t y = 0) noexcept;
        void AddValue(String value) noexcept;
        void SetValue(uint32_t index, String value) noexcept;
        void SetListHeight(int height) noexcept;
        int32_t GetSelectedIndex() const noexcept;
        String GetValue(uint32_t index) noexcept;
        String GetSelectedValue() const noexcept;
        void Clear() noexcept; //!< Deletes every item in the list.

    private:
        void pDraw(RenderTarget& target) noexcept override;
        Listbox* m_listComponent;
        TextInput* m_textInput;
        Color m_borderColor, m_bodyColor;
        double m_borderAnimationProgress = 0;
        float m_roundness = 0;
    };
}