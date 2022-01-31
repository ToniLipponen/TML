#include <TML/Graphics/Camera.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tml
{
    Camera::Camera() = default;

    Camera::Camera(const Vector2f &pos, float zoom, float rotation)
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

    Vector2f Camera::ScreenToWorld(const Vector2f &point) const noexcept
    {
        static int f[4];
        glad_glGetIntegerv(GL_VIEWPORT, f);
        glm::mat4 m = glm::mat4(1.0f);
        const auto t = glm::translate(glm::mat4(1.0f), glm::vec3(-m_pos.x, -m_pos.y, 0));
        const auto r = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0.f, 0.f, 1.f));
        const auto s = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoom, m_zoom, 1.f));
        const auto o = glm::translate(glm::mat4(1.0f), glm::vec3(f[2]/2.f, f[3]/2.f, 0));
        m = o*r*t*s;
        m = glm::inverse(m);
        const auto res = m * glm::vec4(point.x, point.y, -1, 1);

        return Vector2f{res.x, res.y} + (m_pos - (m_pos / m_zoom));
    }

    Vector2f Camera::WorldToScreen(const Vector2f &point) const noexcept
    {
        int f[4];
        glad_glGetIntegerv(GL_VIEWPORT, f);
        const auto n = glm::mat4(1.0f);
        const auto t = glm::translate(n, glm::vec3(-m_pos.x, -m_pos.y, 0));
        const auto r = glm::rotate(n, m_rotation, glm::vec3(0.f, 0.f, 1.f));
        const auto s = glm::scale(n, glm::vec3(m_zoom, m_zoom, 1.f));
        const auto o = glm::translate(n, glm::vec3(f[2]/2.f, f[3]/2.f, 0));
        const glm::mat4 m = o*r*t*s;
        const auto res = m * glm::vec4(point.x, point.y, -1, 1);
        return Vector2f{res.x, res.y} + (m_pos - (m_pos / m_zoom)) * -1.f * m_zoom;
    }
}