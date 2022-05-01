#pragma once
#include <TML/System/Math/Vector2.h>

namespace tml
{
    class Camera
    {
    public:
        Camera();
        Camera(const tml::Vector2f& pos, float zoom = 1.0f, float rotation = 0.0f);

        void Move(const Vector2f& offset);
        void Zoom(float amount);
        void Rotate(float degrees);

        void SetPosition(const Vector2f& position);
        void SetZoom(float zoomLevel); //!< 1.0 is normal. Bigger value is more zoom.
        void SetRotation(float degrees);

        Vector2f GetPosition() const noexcept;
        float GetZoom() const noexcept;
        float GetRotation() const noexcept;
        Vector2f ScreenToWorld(const Vector2f& point, const Vector2f& size) const noexcept;
        Vector2f WorldToScreen(const Vector2f& point, const Vector2f& size) const noexcept;

    private:
        Vector2f m_pos = {0,0};    //!< Top left position of the camera view.
        float m_zoom = 1.0f;
        float m_rotation = 0.0f;   //!< Rotation in degrees.
    };
}
