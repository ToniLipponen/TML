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
            Viewport(i32 x, i32 y, ui32 w, ui32 h);
            RenderTexture& GetSurface() { return m_surface; }
        protected:
            void pDraw(Renderer &window) override;
            RenderTexture m_surface;
        private:
            void SetCallbacks();
        };
    }
}