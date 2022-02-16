#pragma once
#include <TML/Graphics/Drawable/Drawable.h>

namespace tml
{
    class Shape : public Drawable
    {
    public:
        struct ShapePoint { Vector2f pos; Color color;};
    public:
        Shape() = default;

        ui32 AddPoint(const Vector2f& position) noexcept;
        ui32 AddPoint(const Vector2f& position, const Color& color) noexcept;
        ui32 AddPoint(const ShapePoint& point) noexcept;

        bool SetPoint(ui32 index, const Vector2f& position) noexcept;
        bool SetPoint(ui32 index, const Vector2f& position, const Color& color) noexcept;
        bool SetPoint(ui32 index, const ShapePoint& point);

        bool RemovePoint(ui32 index) noexcept;
        bool ClearPoints() noexcept;

        ui32 GetPointCount() const noexcept;

    private:
        void Generate() noexcept override;

    protected:
        std::vector<ShapePoint> m_points;
    };
}