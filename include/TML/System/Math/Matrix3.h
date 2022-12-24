#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace tml
{
    template<typename T>
    class Matrix3
    {
    public:
        [[maybe_unused]]
        Matrix3() noexcept = default;

        [[maybe_unused]]
        Matrix3(T s0, T s1, T s2,
                T s3, T s4, T s5,
                T s6, T s7, T s8) noexcept
        {
            m_cols[0] = Vector3<T>(s0, s3, s6);
            m_cols[1] = Vector3<T>(s1, s4, s7);
            m_cols[2] = Vector3<T>(s2, s5, s8);
        }

        [[maybe_unused]]
        Matrix3(const Vector3<T>& row0,
                const Vector3<T>& row1,
                const Vector3<T>& row2) noexcept
        {
            m_cols[0] = { row0.x, row1.x, row2.x };
            m_cols[1] = { row0.y, row1.y, row2.y };
            m_cols[2] = { row0.z, row1.z, row2.z };
        }

        [[maybe_unused, nodiscard]]
        Vector3<T>& operator[](unsigned int index) noexcept
        {
            return m_cols[index];
        }

        [[maybe_unused, nodiscard]]
        const Vector3<T>& operator[](unsigned int index) const noexcept
        {
            return m_cols[index];
        }

        [[maybe_unused, nodiscard]]
        constexpr Matrix3<T> operator*(const Matrix3<T>& other) const noexcept
        {
            Matrix3<T> result{};

            for(unsigned int i = 0; i < 3; ++i)
            {
                for(unsigned int j = 0; j < 3; ++j)
                {
                    Vector3<T> row{other[0][j], other[1][j], other[2][j]};

                    result[i][j] = row.Dot(m_cols[i]);
                }
            }

            return result;
        }

        [[maybe_unused, nodiscard]]
        constexpr Matrix3<T> operator*(T scalar) const noexcept
        {
            Matrix3<T> result{};

            result.m_cols[0] = m_cols[0] * scalar;
            result.m_cols[1] = m_cols[1] * scalar;
            result.m_cols[2] = m_cols[2] * scalar;

            return result;
        }

        [[maybe_unused, nodiscard]]
        constexpr Vector3<T> operator*(const Vector3<T>& vector) const noexcept
        {
            Vector3<T> result{};

            result[0] = vector.Dot(m_cols[0]);
            result[1] = vector.Dot(m_cols[1]);
            result[2] = vector.Dot(m_cols[2]);

            return result;
        }

        [[maybe_unused, nodiscard]]
        static constexpr Matrix3<T> Identity() noexcept
        {
            return
            {
                1, 0, 0,
                0, 1, 0,
                0, 0, 1
            };
        }

        [[maybe_unused, nodiscard]]
        static constexpr Matrix3<T> Translate(const Vector2<T>& offset) noexcept
        {
            return
            {
                1, 0, offset.x,
                0, 1, offset.y,
                0, 0, 1,
            };
        }

        [[maybe_unused, nodiscard]]
        static constexpr Matrix3<T> Scale(const Vector2<T>& scale) noexcept
        {
            return
            {
                scale.x, 0, 0,
                0, scale.y, 0,
                0, 0, 1,
            };
        }

        [[maybe_unused, nodiscard]]
        static constexpr Matrix3<T> Rotate(const Vector3<T>& axis, float r) noexcept
        {
            r *= 0.01745329252;

            const auto sinr = static_cast<T>(sinf(r));
            const auto cosr = static_cast<T>(cosf(r));
            const auto zero = static_cast<T>(0);
            const auto one  = static_cast<T>(1);

            if(axis[0] > 0.001)
            {
                return Matrix3<T>(
                    Vector3<T>(one, zero, zero),
                    Vector3<T>(zero, cosr, -sinr),
                    Vector3<T>(zero, sinr, cosr));
            }
            else if(axis[1] > 0.001)
            {
                return Matrix3<T>(
                    Vector3<T>(cosr, zero, sinr),
                    Vector3<T>(zero, one, zero),
                    Vector3<T>(-sinr, zero, cosr));
            }
            else
            {
                return Matrix3<T>(
                    Vector3<T>(cosr, -sinr, zero),
                    Vector3<T>(sinr, cosr, zero),
                    Vector3<T>(zero, zero, one));
            }
        }

        [[maybe_unused, nodiscard]]
        static constexpr Matrix3<T> Inverse(const Matrix3<T>& m) noexcept
        {
            T oneOverDeterminant = static_cast<T>(1) / (
                    m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
                    - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
                    + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

            Matrix3<T> result;
            result[0][0] =  (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * oneOverDeterminant;
            result[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * oneOverDeterminant;
            result[2][0] =  (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * oneOverDeterminant;
            result[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * oneOverDeterminant;
            result[1][1] =  (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * oneOverDeterminant;
            result[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * oneOverDeterminant;
            result[0][2] =  (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * oneOverDeterminant;
            result[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * oneOverDeterminant;
            result[2][2] =  (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * oneOverDeterminant;

            return result;
        }
    private:
        Vector3<T> m_cols[3];
    };

    using Matrix3f [[maybe_unused]] = Matrix3<float>;
}