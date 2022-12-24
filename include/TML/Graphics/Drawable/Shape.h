#pragma once
#include <TML/Graphics/Drawable/Drawable.h>
#include <TML/Graphics/Core/Texture.h>

namespace tml
{
    struct TML_API ShapePoint
    {
        explicit ShapePoint(const Vector2f& position)
        : pos(position), uv(0,0), color(Color::White)
        {

        }

        ShapePoint(const Vector2f& position, const Color& color)
        : pos(position), uv(0,0), color(color)
        {

        }

        ShapePoint(const Vector2f& position, const Vector2f& uv, const Color& color)
        : pos(position), uv(uv), color(color)
        {

        }

        Vector2f pos;
        Vector2f uv;
        Color color;
    };

    class TML_API Shape : public Drawable
    {
    public:
        Shape() = default;
        explicit Shape(std::vector<ShapePoint> points) noexcept;
        Shape(std::vector<ShapePoint> points, const Texture& texture) noexcept;
        Shape(std::vector<ShapePoint> points, std::shared_ptr<Texture> texture) noexcept;

        uint32_t AddPoint(const ShapePoint& point) noexcept;
        bool SetPoint(uint32_t index, const ShapePoint& point) noexcept;
        bool RemovePoint(uint32_t index) noexcept;
        bool ClearPoints() noexcept;
        Color SetColor(const Color& color) noexcept override;
        void SetTexture(const Texture& texture) noexcept;
        void SetTexture(const std::shared_ptr<Texture>& texture) noexcept;

        [[nodiscard]] uint32_t GetPointCount() const noexcept;

    private:
        void OnDraw(class RenderTarget* renderer, class Texture* circleTexture) noexcept override;

    protected:
        std::vector<ShapePoint> m_points;
        std::shared_ptr<Texture> m_texture = nullptr;
    };
}