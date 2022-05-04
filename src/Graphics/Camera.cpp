#include <TML/Graphics/Camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tml
{
    Camera::Camera() = default;

    Camera::Camera(const Vector2f& pos, float zoom, float rotation)
    : m_pos(pos), m_zoom(zoom), m_rotation(rotation)
    {

    }

    void Camera::Move(const Vector2f &offset)
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

    void Camera::SetZoom(float zoom_level)
    {
        m_zoom = zoom_level;
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

    Vector2f Camera::ScreenToWorld(const Vector2f &point, const Vector2f& size) const noexcept
    {
        const auto size2 = size / 2;
        auto n = glm::mat4(1.0f);
        n = glm::translate(n, glm::vec3(-m_pos.x, -m_pos.y, 0));
        n = glm::rotate(n, m_rotation, glm::vec3(0.f, 0.f, 1.f));
        n = glm::scale(n, glm::vec3(m_zoom, m_zoom, 1.f));
        n = glm::translate(glm::mat4(1.0f), glm::vec3(size2.x, size2.y, 0)) * n;
        const auto res = glm::inverse(n) * glm::vec4(point.x, point.y, -1, 1);
        return Vector2f{res.x, res.y} + (m_pos - (m_pos / m_zoom)) + size2;
    }

    Vector2f Camera::WorldToScreen(const Vector2f &point, const Vector2f& size) const noexcept
    {
        const auto size2 = size / 2;
        auto n = glm::mat4(1.0f);
        n = glm::translate(n, glm::vec3(-m_pos.x, -m_pos.y, 0));
        n = glm::rotate(n, m_rotation, glm::vec3(0.f, 0.f, 1.f));
        n = glm::scale(n, glm::vec3(m_zoom, m_zoom, 1.f));
        n = glm::translate(n, glm::vec3(size2.x, size2.y, 0));
        const auto res = n * glm::vec4(point.x, point.y, -1, 1);
        return (Vector2f{res.x, res.y} + (m_pos - (m_pos / m_zoom)) * -1.f * m_zoom) - size2;
    }
}