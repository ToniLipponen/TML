#pragma once
#include <TML/Graphics/Drawable/Drawable.h>

namespace tml
{
    class TML_API Shape : public Drawable
    {
    public:
        struct ShapePoint { Vector2f pos; Color color;};
    public:
        Shape() = default;

        uint32_t AddPoint(const Vector2f& position) noexcept;
        uint32_t AddPoint(const Vector2f& position, const Color& color) noexcept;
        uint32_t AddPoint(const ShapePoint& point) noexcept;

        bool SetPoint(uint32_t index, const Vector2f& position) noexcept;
        bool SetPoint(uint32_t index, const Vector2f& position, const Color& color) noexcept;
        bool SetPoint(uint32_t index, const ShapePoint& point);

        bool RemovePoint(uint32_t index) noexcept;
        bool ClearPoints() noexcept;

        uint32_t GetPointCount() const noexcept;

    private:
        void OnDraw(class Renderer*, class Texture*) noexcept override;

    protected:
        std::vector<ShapePoint> m_points;
    };
}