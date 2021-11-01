#pragma once
#include "../InterfaceComponent.h"
#include "Listbox.h"
#include <string>

namespace tml
{
    namespace Interface
    {
        class DropMenu : public BaseComponent
        {
        public:
            DropMenu(i32 x, i32 y, ui32 width, ui32 height = 20);
            void AddValue(std::string value);
            void SetValue(ui32 index, std::string value);
            i32 GetSelectedIndex() const;
            std::string GetValue(ui32 index);
            std::string GetSelectedValue() const;
            void Clear();
        private:
            void Draw() override;
            virtual void OnUpdate(double dt) override;
            virtual void OnMouseClick(const Vector2& mousePos) override;
            virtual void OnMouseDown(const Vector2& mousePos) override;
            virtual void OnFocused() override;
            virtual void OnFocusLost() override;

            Listbox* m_listComponent;
        };
    }
}