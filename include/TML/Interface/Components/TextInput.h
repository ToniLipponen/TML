#pragma once
#include "../BaseComponent.h"
#include <TML/Utilities/Utilities.h>

namespace tml
{
    namespace Interface
    {
        class TextInput : public BaseComponent
        {
        public:
            TextInput(i32 x, i32 y, i32 width, i32 height = 30);
            void SetValue(const std::string& string){ m_value = Util::StringToWstring(string); m_cursorIndex = string.size(); }
            std::string GetValue() const { return Util::WstringToString(m_value); }
        private:
            void OnMouseClick(const Vector2& p) override;
            void OnUpdate(double dt) override;
            void Draw() override;
            std::wstring m_value;
            ui32 m_cursorIndex = 0;
            double m_blinkTimer = 0;
            bool m_showLine = true;
        };
    }
}