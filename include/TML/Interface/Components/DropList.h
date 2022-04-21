#pragma once
#include "../BaseComponent.h"
#include "Listbox.h"
#include <string>

namespace tml
{
    namespace Interface
    {
        class DropList : public BaseComponent
        {
        public:
            DropList(int32_t x, int32_t y, uint32_t width, uint32_t height = 20);
            void AddValue(String value);
            void SetValue(uint32_t index, String value);
            void SetListHeight(int height);
            int32_t GetSelectedIndex() const;
            String GetValue(uint32_t index);
            String GetSelectedValue() const;
            void Clear(); //!< Deletes every item in the list.
        private:
            void pDraw(Renderer &renderer) override;
            Listbox* m_listComponent;
        };
    }
}