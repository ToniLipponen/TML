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
            DropList(i32 x, i32 y, ui32 width, ui32 height = 20);
            void AddValue(String value);
            void SetValue(ui32 index, String value);
            void SetListHeight(int height);
            i32 GetSelectedIndex() const;
            String GetValue(ui32 index);
            String GetSelectedValue() const;
            void Clear(); //!< Deletes every item in the list.
        private:
            void pDraw(Renderer &renderer) override;
            Listbox* m_listComponent;
        };
    }
}