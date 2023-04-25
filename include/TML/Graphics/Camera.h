#pragma once
#include <TML/System/Math/Matrix4.h>
#include <TML/System/Math/Vector2.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Camera
    {
    public:
        Camera();

        explicit Camera(tml::Vector2f pos, float zoom = 1.0f, float rotation = 0.0f);

        void Move(const Vector2f& offset);

        /** Multiplicative zoom. The zoom level of the camera is multiplied by amount. */
        void Zoom(float amount);

        void Rotate(float degrees);
        
        void SetPosition(const Vector2f& position);

        /** Sets the zoom of the camera. Bigger value means more zoom. 1.0 is normal / default zoom level. */
        void SetZoom(float zoomLevel);

        /** Sets the rotation of the camera in degrees. */
        void SetRotation(float degrees);

        [[nodiscard]] Vector2f GetPosition() const noexcept;

        /** @return The zoom level of the camera. A bigger value means more zoom. */
        [[nodiscard]] float GetZoom() const noexcept;

        /** @return The rotation of the camera in degrees. */
        [[nodiscard]] float GetRotation() const noexcept;

        /** Transforms a screen coordinate to world coordinate.
         * @param point The point you wish to transform.
         * @param size The size of the RenderTarget you are drawing on.
         * @see tml::RenderTarget::ScreenToWorld()
         * @return Transformed coordinate. */
        [[nodiscard]] Vector2f ScreenToWorld(const Vector2f& point, const Vector2f& size) const noexcept;

        /**
         * Transforms a world coordinate to screen coordinate.
         * @param point The point you wish to transform.
         * @param size The size of the RenderTarget you are drawing on.
         * @return Transformed coordinate. */
        [[nodiscard]] Vector2f WorldToScreen(const Vector2f& point, const Vector2f& size) const noexcept;

        /**
         * Gets the whole view matrix of the camera.
         * @return The view matrix of the camera. */
        [[nodiscard]] Matrix4f GetMatrix() const noexcept;

    private:
        Vector2f m_pos; //!< Top left position of the camera view.
        float m_zoom = 1.0f;
        float m_rotation = 0.0f; //!< Rotation in degrees.
    };
}

/** @class tml::Camera
  * @ingroup Graphics
  * A 2D representation of a camera.
  * You can move, rotate and scale the rendered view using the camera.
  */
