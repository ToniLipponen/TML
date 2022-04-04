#pragma once
#include <TML/Graphics/Camera.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    class RenderTarget : public Renderer
    {
    public:
        void SetCamera(const Camera& camera);
        void ResetCamera();
        void SetViewport(const Vector2i& pos, const Vector2i& size) noexcept override;
        Vector2f WorldToScreen(const Vector2f& point, const Camera& camera) const noexcept;
        Vector2f ScreenToWorld(const Vector2f& point, const Camera& camera) const noexcept;

    private:
        virtual Vector2f GetRenderTargetSize() const noexcept = 0;
    };
}
