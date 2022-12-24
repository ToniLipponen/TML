#pragma once
#include "Vector3.h"
#include "Vector4.h"

namespace tml
{
    template<typename T>
    class Matrix4
    {
    public:
        [[maybe_unused]]
        Matrix4() noexcept = default;

        [[maybe_unused]]
        Matrix4(T s0,  T s1,  T s2,  T s3,
                T s4,  T s5,  T s6,  T s7,
                T s8,  T s9,  T s10, T s11,
                T s12, T s13, T s14, T s15) noexcept
        {
            m_cols[0] = { s0, s4, s8,  s12 };
            m_cols[1] = { s1, s5, s9,  s13 };
            m_cols[2] = { s2, s6, s10, s14 };
            m_cols[3] = { s3, s7, s11, s15 };
        }

        [[maybe_unused]]
        Matrix4(const Vector4<T>& row0,
                const Vector4<T>& row1,
                const Vector4<T>& row2,
                const Vector4<T>& row3) noexcept
        {
            m_cols[0] = {row0.x, row1.x, row2.x, row3.x};
            m_cols[1] = {row0.y, row1.y, row2.y, row3.y};
            m_cols[2] = {row0.z, row1.z, row2.z, row3.z};
            m_cols[3] = {row0.w, row1.w, row2.w, row3.w};
        }

        [[maybe_unused, nodiscard]]
        Vector4<T>& operator[](unsigned int index) noexcept
        {
            return m_cols[index];
        }

        [[maybe_unused, nodiscard]]
        const Vector4<T>& operator[](unsigned int index) const noexcept
        {
            return m_cols[index];
        }

        [[maybe_unused, nodiscard]]
        constexpr Matrix4<T> operator*(const Matrix4<T>& other) const noexcept
        {
            Matrix4<T> result{};

            for(unsigned int i = 0; i < 4; ++i)
            {
                for(unsigned int j = 0; j < 4; ++j)
                {
                    Vector4<T> row{other[0][j], other[1][j], other[2][j], other[3][j]};

                    result[i][j] = row.Dot(m_cols[i]);
                }
            }

            return result;
        }

        [[maybe_unused, nodiscard]]
        constexpr Matrix4<T> operator*(T scalar) const noexcept
        {
            Matrix4<T> result{};

            result.m_cols[0] = m_cols[0] * scalar;
            result.m_cols[1] = m_cols[1] * scalar;
            result.m_cols[2] = m_cols[2] * scalar;
            result.m_cols[3] = m_cols[3] * scalar;

            return result;
        }

        [[maybe_unused, nodiscard]]
        constexpr Vector4<T> operator*(const Vector4<T>& vector) const noexcept
        {
            Vector4<T> result{};

            result[0] = vector.Dot(m_cols[0]);
            result[1] = vector.Dot(m_cols[1]);
            result[2] = vector.Dot(m_cols[2]);
            result[3] = vector.Dot(m_cols[3]);

            return result;
        }

        [[maybe_unused, nodiscard]]
        static Matrix4<float> Orthographic(
                float left,
                float right,
                float top,
                float bottom,
                float near,
                float far) noexcept
        {
            const auto two = static_cast<float>(2);
            const auto zero = static_cast<float>(0);
            const auto one = static_cast<float>(1);

            return {
                two / (right - left), zero, zero, zero,
                zero, two / (top - bottom), zero, zero,
                zero, zero, -two / (far - near), zero,
                -(right + left) / (right - left), -(top + bottom) / (top - bottom),
                -(far + near) / (far - near), one
            };
        }

        [[maybe_unused, nodiscard]]
        constexpr static Matrix4<T> Identity() noexcept
        {
            return {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }

        [[maybe_unused, nodiscard]]
        constexpr static Matrix4<T> Translate(const Vector3<T>& offset) noexcept
        {
            return {
                1, 0, 0, offset.x,
                0, 1, 0, offset.y,
                0, 0, 1, offset.z,
                0, 0, 0, 1
            };
        }

        [[maybe_unused, nodiscard]]
        constexpr static Matrix4<T> Scale(const Vector3<T>& scale) noexcept
        {
            return {
                scale.x, 0, 0, 0,
                0, scale.y, 0, 0,
                0, 0, scale.z, 0,
                0, 0, 0, 1
            };
        }

        [[maybe_unused, nodiscard]]
        inline static Matrix4<T> Rotate(const Vector3<T>& axis, float r) noexcept
        {
            r *= 0.01745329252;

            const auto sinr = static_cast<T>(sinf(r));
            const auto cosr = static_cast<T>(cosf(r));
            const auto zero = static_cast<T>(0);
            const auto one  = static_cast<T>(1);

            if(axis[0] > 0.001)
            {
                return {
                    one, zero, zero, zero,
                    zero, cosr, -sinr, zero,
                    zero, sinr, cosr, zero,
                    zero, zero, zero, one
                };
            }
            else if(axis[1] > 0.001)
            {
                return {
                    cosr, zero, sinr, zero,
                    zero, one, zero, zero,
                    -sinr, zero, cosr, zero,
                    zero, zero, zero, one
                };
            }
            else
            {
                return {
                    cosr, -sinr, zero, zero,
                    sinr, cosr, zero, zero,
                    zero, zero, one, zero,
                    zero, zero, zero, one
                };
            }
        }

        [[maybe_unused, nodiscard]]
        static Matrix4<T> Translate(const Matrix4<T>& matrix, const Vector3<T>& vector) noexcept
        {
            Matrix4<T> result{matrix};
            result[0][3] += vector.x;
            result[1][3] += vector.y;
            result[2][3] += vector.z;

            return result;
        }

        [[maybe_unused, nodiscard]]
        static Matrix4<T> Rotate(const Matrix4<T>& matrix, const Vector3<T>& vector, float angle) noexcept
        {
            return matrix * Matrix4<T>::Rotate(vector, angle);
        }

        [[maybe_unused, nodiscard]]
        static Matrix4<T> Scale(const Matrix4<T>& matrix, const Vector3<T>& vector) noexcept
        {
            return matrix * Matrix4<T>::Scale(vector);
        }

        [[maybe_unused, nodiscard]]
        inline constexpr static Matrix4<T> Inverse(const Matrix4<T>& m) noexcept
        {
            const T coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            const T coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
            const T coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

            const T coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            const T coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
            const T coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

            const T coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            const T coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
            const T coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

            const T coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            const T coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
            const T coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

            const T coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            const T coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
            const T coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

            const T coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            const T coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
            const T coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

            const Vector4<T> fac0(coef00, coef00, coef02, coef03);
            const Vector4<T> fac1(coef04, coef04, coef06, coef07);
            const Vector4<T> fac2(coef08, coef08, coef10, coef11);
            const Vector4<T> fac3(coef12, coef12, coef14, coef15);
            const Vector4<T> fac4(coef16, coef16, coef18, coef19);
            const Vector4<T> fac5(coef20, coef20, coef22, coef23);

            const Vector4<T> vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
            const Vector4<T> vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
            const Vector4<T> vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
            const Vector4<T> vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

            const Vector4<T> inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
            const Vector4<T> inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
            const Vector4<T> inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
            const Vector4<T> inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

            const Vector4<T> signA(
                static_cast<T>( 1),
                static_cast<T>(-1),
                static_cast<T>( 1),
                static_cast<T>(-1)
            );

            const Vector4<T> signB(
                static_cast<T>(-1),
                static_cast<T>( 1),
                static_cast<T>(-1),
                static_cast<T>( 1)
            );

            const Matrix4<T> inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);
            const Vector4<T> row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);
            const Vector4<T> dot0(m[0] * row0);
            const T dot1 = (dot0[0] + dot0[1]) + (dot0[2] + dot0[3]);
            const T oneOverDeterminant = static_cast<T>(1) / dot1;

            return inverse * oneOverDeterminant;
        }
    private:
        Vector4<T> m_cols[4];
    };

    using Matrix4f [[maybe_unused]] = Matrix4<float>;
}