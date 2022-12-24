#pragma once
#include "Vector2.h"

namespace tml
{
    template<typename T>
    class Matrix2
    {
    public:
        Matrix2() noexcept = default;

        [[maybe_unused]]
        Matrix2(T s0, T s1,
                T s2, T s3) noexcept
        {
            m_cols[0] = {s0, s2};
            m_cols[1] = {s1, s3};
        }

        [[maybe_unused]]
        Matrix2(const Vector2<T>& row0, const Vector2<T>& row1) noexcept
        {
            m_cols[0] = {row0.x, row1.x};
            m_cols[1] = {row0.y, row1.y};
        }

        [[maybe_unused, nodiscard]]
        Vector2<T>& operator[](unsigned int index) noexcept
        {
            return m_cols[index];
        }

        [[maybe_unused, nodiscard]]
        Vector2<T> operator[](unsigned int index) const noexcept
        {
            return m_cols[index];
        }

        [[maybe_unused, nodiscard]]
        Matrix2<T> operator*(const Matrix2<T>& other) noexcept
        {
            Matrix2<T> result;

            result.m_cols[0] = Vector2<T>(other.m_cols[0][0], other.m_cols[1][0]).Dot();
            result.m_cols[0] = Vector2<T>(other.m_cols[0][1], other.m_cols[1][1]).Dot();

            return result;
        }

        [[maybe_unused, nodiscard]]
        Vector2<T> operator*(const Vector2<T>& vector) noexcept
        {
            return vector * m_cols[0] + vector * m_cols[1];
        }

        [[maybe_unused, nodiscard]]
        static constexpr Matrix2<T> Identity() noexcept
        {
            return
            {
                1, 0,
                0, 1
            };
        }

        [[maybe_unused, nodiscard]]
        static Matrix2<T> Rotate(float r) noexcept
        {
            r *= 0.01745329252;
            const auto sinr = static_cast<T>(sinf(r));
            const auto cosr = static_cast<T>(cosf(r));

            return Matrix2<T>(
                Vector2<T>(cosr, -sinr),
                Vector2<T>(sinr, cosr));
        }

        [[maybe_unused, nodiscard]]
        constexpr static Matrix2<T> Inverse(const Matrix2<T>& m) noexcept
        {
            T oneOverDeterminant = static_cast<T>(1) / (
                    m[0][0] * m[1][1]
                    -m[1][0] * m[0][1]);

            return Matrix2<T>(
                    m[1][1] * oneOverDeterminant,
                    -m[0][1] * oneOverDeterminant,
                    -m[1][0] * oneOverDeterminant,
                    m[0][0] * oneOverDeterminant);
        }

    private:
        Vector2<T> m_cols[2];
    };

    using Matrix2f [[maybe_unused]] = Matrix2<float>;
}