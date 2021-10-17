#pragma once
#include "Types.h"

namespace tml
{
    class Camera
    {
    public:
        Camera();
        Camera(const tml::Vector2& pos, float zoom = 1.0f, float rotation = 0.0f);
        void Move(const Vector2& offset);
        void Zoom(float amount); // Additive
        void Rotate(float degrees);

        void SetPosition(const Vector2& position);
        void SetZoom(float zoom_level); // 1.0 is normal. Bigger value is more zoom
        void SetRotation(float degrees);

        Vector2 GetPosition() const noexcept;
        float GetZoom() const noexcept;
        float GetRotation() const noexcept;
        Vector2 ScreenToWorld(const Vector2& point);
    private:
        Vector2 m_pos = {0,0}; // Camera is centered on this
        float m_zoom = 1.0f;
        float m_rotation = 0.0f;
    };
}
