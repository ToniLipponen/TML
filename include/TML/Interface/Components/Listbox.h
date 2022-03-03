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
            void AddValue(String value);
            void SetValue(ui32 index, String value); // Set index to value.
            String GetValue(ui32 index); // @brief Returns an empty string if index is greater than list size - 1.
            String GetSelectedValue() const;
            i32 GetSelectedIndex() const;
            ui32 GetElementsCount() const;
            bool ContainsValue(const std::string& value) const;
            void RemoveValue(ui32 index);
            void Clear();
            friend class Dropmenu;
        private:
            i32 GetOverFlow() const noexcept; // Get count of items that do not fit int the visible area.
        protected:
            void pDraw(Renderer &renderer) override;
            std::vector<String> m_values;
            Scrollbar<Vertical>* m_scrollbar = nullptr;

            float m_scrollDist = 0.f;
            i32 m_selectedIndex = -1;
        };
    }
}