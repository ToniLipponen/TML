#pragma once
#include "../InterfaceComponent.h"

namespace tml::Interface
{
    class TextInput : public BaseComponent
    {
    public:
        TextInput(i32 x, i32 y, i32 width, i32 height = 30);
        void SetValue(const std::string& string){ m_value = string; }
        std::string GetValue() const { return m_value; }
    private:
        void OnMouseClick(const Vector2& p) override;
        void OnUpdate(float dt) override;
        void Draw() override;
        std::string m_value;
        float m_repeatTimer;
    };
}