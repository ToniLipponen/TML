#pragma once
#include "../InterfaceComponent.h"
#include <TML/Utilities/Utilities.h>

namespace tml::Interface
{
    class TextInput : public BaseComponent
    {
    public:
        TextInput(i32 x, i32 y, i32 width, i32 height = 30);
        void SetValue(const std::string& string){ m_value = Util::stringToWstring(string); m_cursorIndex = string.size(); }
        std::string GetValue() const { return Util::wstringToString(m_value); }
    private:
        void OnMouseClick(const Vector2& p) override;
        void OnUpdate(float dt) override;
        void Draw() override;
        std::wstring m_value;
        ui32 m_cursorIndex = 0;
        float m_repeatTimer;
    };
}