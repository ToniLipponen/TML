#pragma once
#include "../BaseComponent.h"
#include <TML/Interface/Components/Scrollbar.h>
#include <vector>

namespace tml
{
    namespace Interface
    {
        class Listbox : public BaseComponent
        {
        public:
            Listbox(i32 x, i32 y, ui32 width, ui32 height);
            void AddValue(std::string value);
            void SetValue(ui32 index, std::string value); // Set index to value.
            std::string GetValue(ui32 index); // @brief Returns an empty string if index is greater than list size - 1.
            std::string GetSelectedValue() const;
            i32 GetSelectedIndex() const;
            ui32 GetElementsCount() const;
            bool ContainsValue(const std::string& value) const;
            void RemoveValue(ui32 index);
            void Clear();
            friend class DropMenu;
        private:
            ui32 GetOverFlow() const noexcept; // Get count of items that do not fit int the visible area.
        protected:
            void OnMoved() override;
            void OnResized() override;
            void OnMouseClick(const Vector2i& mousePos) override;
            void OnUpdate(double dt) override;
            void Draw() override;
            std::vector<std::string> m_values;
            Scrollbar<Vertical>* m_scrollbar = nullptr;

            float m_scrollDist = 0.f;
            i32 m_selectedIndex = -1;
        };
    }
}