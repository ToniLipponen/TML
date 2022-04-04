#include <TML/Graphics/RenderTarget.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

namespace tml
{
    void RenderTarget::SetCamera(const Camera& camera)
    {
        ResetCamera();
        const auto pos = camera.GetPosition();
        const auto zoom = camera.GetZoom();

        auto* view = reinterpret_cast<glm::mat4*>(m_view);

        *view = glm::rotate(*view, camera.GetRotation(), glm::vec3(0.f, 0.f, 1.f));
        *view = glm::scale(*view, glm::vec3(zoom, zoom, 0));
        *view = glm::translate(*view, glm::vec3(-pos.x, -pos.y, 0));
    }

    void RenderTarget::ResetCamera()
    {
        EndBatch();
        BeginBatch();
        auto* view = reinterpret_cast<glm::mat4*>(m_view);
        auto* scale = reinterpret_cast<glm::mat4*>(m_scale);

        *view  = glm::mat4(1.0f);
        *scale = glm::mat4(1.0f);
    }

    void RenderTarget::SetViewport(const Vector2i &pos, const Vector2i &size) noexcept
    {
        const auto targetSize = static_cast<Vector2i>(GetRenderTargetSize());
        Renderer::SetViewport(Vector2i(pos.x, -pos.y + targetSize.y - size.y), size);
    }

    Vector2f RenderTarget::WorldToScreen(const Vector2f& point, const Camera& camera) const noexcept
    {
        return camera.WorldToScreen(point, GetRenderTargetSize());
    }

    Vector2f RenderTarget::ScreenToWorld(const Vector2f& point, const Camera& camera) const noexcept
    {
        return camera.ScreenToWorld(point, GetRenderTargetSize());
    }
}