#include <TML/Graphics/Camera.h>

namespace tml
{
    Camera::Camera() = default;

    Camera::Camera(Vector2f pos, float zoom, float rotation)
    : m_pos(pos), m_zoom(zoom), m_rotation(rotation)
    {

    }

    void Camera::Move(const Vector2f& offset)
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

    void Camera::SetPosition(const Vector2f& position)
    {
        m_pos = position;
    }

    void Camera::SetZoom(float zoomLevel)
    {
        m_zoom = zoomLevel;
    }

    void Camera::SetRotation(float d)
    {
        m_rotation = d;
    }

    Vector2f Camera::GetPosition() const noexcept
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

    Vector2f Camera::ScreenToWorld(const Vector2f& point, const Vector2f& size) const noexcept
    {
        const auto t = Matrix4f::Translate(Vector3f(-m_pos.x, -m_pos.y, 0.f));
        const auto s = Matrix4f::Scale(Vector3f(m_zoom, m_zoom, 1.f));
        const auto r = Matrix4f::Rotate(Vector3f(0.f, 0.f, 1.f), -m_rotation);
        const auto o = Matrix4f::Translate({size.x / 2.f, size.y / 2.f, 0.f});
        const Vector4f res = tml::Matrix4f::Inverse(t * r * s * o) * Vector4f(point.x, point.y, 0.0f, 1.0f);
        return {res.x, res.y};
    }

    Vector2f Camera::WorldToScreen(const Vector2f &point, const Vector2f& size) const noexcept
    {
        const auto t = Matrix4f::Translate(Vector3f(-m_pos.x, -m_pos.y, 0.f));
        const auto s = Matrix4f::Scale(Vector3f(m_zoom, m_zoom, 1.f));
        const auto r = Matrix4f::Rotate(Vector3f(0.f, 0.f, 1.f), -m_rotation);
        const auto o = Matrix4f::Translate({size.x / 2, size.y / 2, 0.f});
        const Vector4f res = t * r * s * o * Vector4f(point.x, point.y, 0.0f, 1.0f);
        return {res.x, res.y};
    }

    Matrix4f Camera::GetMatrix() const noexcept
    {
        const auto t = Matrix4f::Translate(Vector3f(-m_pos.x, -m_pos.y, 0.f));
        const auto s = Matrix4f::Scale(Vector3f(m_zoom, m_zoom, 1.f));
        const auto r = Matrix4f::Rotate(Vector3f(0.f, 0.f, 1.f), -m_rotation);
        return t * r * s;
    }
}