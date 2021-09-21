#pragma once
#include "../InterfaceComponent.h"

namespace tml
{
    namespace Interface
    {
        class Button : public BaseComponent
        {
        public:
            Button(ui32 x, ui32 y, ui32 w, ui32 h, std::string text = "");
            void SetText(const std::string& str);
        private:
            std::string m_text;
            float m_textSize = 15.f;
            void Draw();
        };
    }
}