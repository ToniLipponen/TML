#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/Drawable/Text.h>

namespace tml
{
    namespace Interface
    {
        class Label : public BaseComponent
        {
        public:
            Label(int32_t x, int32_t y, uint32_t s = 20, std::string str = "Label");
            void SetValue(std::string str);
            virtual bool ContainsPoint(const Vector2i& p) override;
        private:
            virtual void pDraw(Renderer &renderer) override;
            Text m_text;
        };
    }
}