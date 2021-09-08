#include "../include/Camera.h"

namespace tml
{
    Camera::Camera()
    {

    }

    Camera::Camera(const tml::Vector2 &pos, float zoom, float rotation)
    : m_pos(pos), m_zoom(zoom), m_rotation(rotation)
    {

    }

    void Camera::Move(const Vector2 &offset)
    {
        m_pos += offset;
    }

    void Camera::Zoom(float zoom)
    {
        m_zoom += zoom;
    }

    void Camera::Rotate(float d)
    {
        m_rotation += d;
    }

    void Camera::SetPosition(const Vector2& position)
    {
        m_pos = position;
    }

    void Camera::SetZoom(float zoom_level)
    {
        m_zoom = zoom_level;
    }

    void Camera::SetRotation(float d)
    {
        m_rotation = d;
    }

    Vector2 Camera::GetPosition() const noexcept
    {
        return m_pos;
    }

    float Camera::GetZoom() const noexcept
    {
        return m_zoom;
    }

    float Camera::GetRotation() const noexcept
    {
        return m_rotation;
    }
}