#pragma once
#include "../InterfaceComponent.h"

namespace tml::Interface
{
    class TextInput : public BaseComponent
    {
    public:
        TextInput(i32 x, i32 y, i32 width, i32 height = 30);
        void SetValue(std::wstring string){ m_value = std::move(string); m_cursorIndex = string.size()-1; }
        std::wstring GetValue() const { return m_value; }
    private:
        void OnMouseClick(const Vector2& p) override;
        void OnUpdate(float dt) override;
        void Draw() override;
        std::wstring m_value;
        ui32 m_cursorIndex = 0;
        float m_repeatTimer;
    };
}