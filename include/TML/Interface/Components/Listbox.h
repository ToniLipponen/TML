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
            Listbox(int32_t x, int32_t y, uint32_t width, uint32_t height);
            void AddValue(String value);
            void SetValue(uint32_t index, String value);
            String GetValue(uint32_t index); //!< Returns an empty string if index is greater than list size - 1.
            String GetSelectedValue() const;
            int32_t GetSelectedIndex() const;
            uint32_t GetElementsCount() const;
            bool ContainsValue(const std::string& value) const;
            void RemoveValue(uint32_t index);
            void Clear();
            friend class DropList;
        private:
            int32_t GetOverFlow() const noexcept; //!< Get count of items that do not fit int the visible area.
        protected:
            void pDraw(Renderer &renderer) override;
            std::vector<String> m_values;
            Scrollbar<Vertical>* m_scrollbar = nullptr;

            float m_scrollDist = 0.f;
            int32_t m_selectedIndex = -1;
        };
    }
}