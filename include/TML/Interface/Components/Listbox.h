#pragma once
#include "../InterfaceComponent.h"
#include <vector>

namespace tml
{
    namespace Interface
    {
        class Listbox : public BaseComponent
        {
        public:
            Listbox(){}
            Listbox(i32 x, i32 y, ui32 width, ui32 height);
            void AddValue(std::string value);
            void SetValue(ui32 index, std::string value); // Set index to value.
            std::string GetValue(ui32 index); // @brief Returns an empty string if index is greater than list size - 1.
            std::string GetSelectedValue() const;
            i32 GetSelectedIndex() const;
            ui32 GetElementsCount() const;
            void Clear();
            friend class DropMenu;
        protected:
            virtual void OnMouseClick(const Vector2& mousePos) override;
            virtual void OnUpdate(float dt = 1.0f / 60.f) override;
            virtual void Draw() override;
            std::vector<std::string> m_values;
            bool m_showSlider = false; // Scroll bar
            float m_scrollDist = 0.f;
            i32 m_selectedIndex = -1;
        };
    }
}