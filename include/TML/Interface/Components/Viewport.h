#pragma once
#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/RenderTexture.h>

namespace tml
{
    namespace Interface
    {
        class Viewport : public BaseComponent
        {
        public:
            Viewport();
            Viewport(int32_t x, int32_t y, uint32_t w, uint32_t h);
            RenderTexture& GetSurface() { return m_surface; }
        protected:
            void pDraw(Renderer &window) override;
            RenderTexture m_surface;
        private:
            void SetCallbacks();
        };
    }
}