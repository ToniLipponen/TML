#pragma once
#include <TML/Interface/InterfaceComponent.h>

namespace tml::Interface
{
    class Label : public BaseComponent
    {
    public:
        Label(i32 x, i32 y, ui32 s = 20, std::wstring str = L"Label");
        void SetValue(std::wstring str);
        virtual bool ContainsPoint(const Vector2& p) override;
    private:
        virtual void Draw() override;
        std::wstring m_text;
        float m_size = 20;
    };
}