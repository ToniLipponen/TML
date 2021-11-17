#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml
{
    namespace Interface
    {
        class Label : public BaseComponent
        {
        public:
            Label(i32 x, i32 y, ui32 s = 20, std::string str = "Label");
            void SetValue(std::string str);
            virtual bool ContainsPoint(const Vector2& p) override;
        private:
            virtual void Draw() override;
            std::string m_text;
            float m_fontSize = 20;
        };
    }
}