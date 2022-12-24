#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace tml
{
    #if defined(TML_PACK_STRUCT_DATA)
    #pragma pack(push, 1)
    #endif
    template<unsigned int R, unsigned int C, typename T>
    class Matrix
    {
    public:
        constexpr Matrix() noexcept
        {
            static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");
            static_assert(R > 0 && C > 0, "Both dimensions of a matrix have to be more than 0");
        }

        Matrix(const Matrix<R,C,T>& other) noexcept
        {
            m_data = other.m_data;
        }

        Matrix(Matrix<R,C,T>&& other) noexcept
        {
            std::swap(m_data, other.m_data);
        }

        Matrix<R,C,T>& operator=(const Matrix<R,C,T>& other) noexcept
        {
            if(this != &other)
            {
                m_data = other.m_data;
            }

            return *this;
        }

        Matrix<R,C,T>& operator=(Matrix<R,C,T>&& other) noexcept
        {
            m_data = std::move(other.m_data);
            return *this;
        }

        constexpr Vector<C, T>& operator[](unsigned int index) noexcept
        {
            return m_rows.at(index);
        }

        constexpr Vector<C, T> operator[](unsigned int index) const noexcept
        {
            return m_rows.at(index);
        }

        template<unsigned int R2, unsigned C2>
        constexpr Matrix<R,C2,T> operator*(const Matrix<R2,C2,T>& other) const noexcept
        {
            Matrix<R,C2,T> result{};

            for(unsigned int i = 0; i < R; ++i)
            {
                Vector<C, T> row = m_rows[i];

                for(unsigned int j = 0; j < C2; ++j)
                {
                    Vector<R2, T> col{};

                    for(unsigned int y = 0; y < R2; ++y)
                    {
                        col[y] = other[y][j];
                    }

                    result[i][j] = row.Dot(col);
                }
            }

            return result;
        }

        constexpr Matrix<R,C,T> operator*(T scalar) const noexcept
        {
            Matrix<R,C,T> result{};

            for(unsigned int i = 0; i < R; ++i)
            {
                result[i] = m_rows[i] * scalar;
            }

            return result;
        }

        constexpr Vector<R,T> operator*(const Vector<R,T>& vector) const noexcept
        {
            Matrix<R, 1, T> matrix;

            for(unsigned int i = 0; i < R; ++i)
            {
                matrix[i][0] = vector[i];
            }

            matrix = *this * matrix;

            Vector<R,T> result{};

            for(unsigned int i = 0; i < R; ++i)
            {
                result[i] = matrix[i][0];
            }

            return result;
        }

        template<typename ... Args>
        constexpr static Matrix<R,C,T> Create(const Args& ... args) noexcept
        {
            Matrix<R,C,T> result{};
            result.m_rows = {args...};
            return result;
        }

        constexpr static unsigned int rows = R;
        constexpr static unsigned int columns = C;

    protected:
        std::array<T, R*C> m_data;
    };

    #if defined(TML_PACK_STRUCT_DATA)
    #pragma pack(pop)
    #endif

    template<typename T>
    class Matrix4x4 : public Matrix<4,4,T>
    {
    public:
        using Matrix<4,4,T>::Matrix;

        Matrix4x4(
                T s0,  T s1,  T s2,  T s3,
                T s4,  T s5,  T s6,  T s7,
                T s8,  T s9,  T s10, T s11,
                T s12, T s13, T s14, T s15) noexcept
        {
            Matrix<4,4,T>::m_data =
            {
                T s0,  T s1,  T s2,  T s3,
                T s4,  T s5,  T s6,  T s7,
                T s8,  T s9,  T s10, T s11,
                T s12, T s13, T s14, T s15
            };
        }

        Matrix4x4(
                const Vector4<T>& row0,
                const Vector4<T>& row1,
                const Vector4<T>& row2,
                const Vector4<T>& row3) noexcept
        {
            Matrix<4,4,T>::m_data =
            {
                row0.x, row0.y, row0.z, row0.w,
                row1.x, row1.y, row1.z, row1.w,
                row2.x, row2.y, row2.z, row2.w,
                row3.x, row3.y, row3.z, row3.w,
            };
        }

        Matrix4x4(const Matrix<4,4,T>& other) noexcept
        : Matrix<4,4,T>(other)
        {

        }

        static Matrix4x4<float> Orthographic(
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

            return Matrix4x4<float>
            {
                two / (right - left), zero, zero, zero,
                zero, two / (top - bottom), zero, zero,
                zero, zero, -two / (far - near), zero,
                -(right + left) / (right - left), -(top + bottom) / (top - bottom),
                -(far + near) / (far - near), one
            };
        }

        constexpr static Matrix4x4<T> Identity() noexcept
        {
            return Matrix4x4<T>{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
            };
        }

        constexpr static Matrix4x4<T> Translate(const Vector3<T>& offset) noexcept
        {
            return
            {
                1, 0, 0, offset.X(),
                0, 1, 0, offset.Y(),
                0, 0, 1, offset.Z(),
                0, 0, 0, 1,
            };
        }

        constexpr static Matrix4x4<T> Scale(const Vector3<T>& scale) noexcept
        {
            return Matrix4x4<T>{
                scale.X(), 0, 0, 0,
                0, scale.Y(), 0, 0,
                0, 0, scale.Z(), 0,
                0, 0, 0, 1,
            };
        }

        inline static Matrix4x4<T> Rotate(const Vector3<T>& axis, float r) noexcept
        {
    #if defined(TML_USE_DEGREES)
            r *= 0.01745329252;
    #endif
            const auto sinr = static_cast<T>(sin(r));
            const auto cosr = static_cast<T>(cos(r));
            const auto zero = static_cast<T>(0);
            const auto one  = static_cast<T>(1);

            if(axis[0] > 0.001)
            {
                return Matrix4x4<T>(
                    one, zero, zero, zero,
                    zero, cosr, -sinr, zero,
                    zero, sinr, cosr, zero,
                    zero, zero, zero, one
                );
            }
            else if(axis[1] > 0.001)
            {
                return Matrix4x4<T>(
                    cosr, zero, sinr, zero,
                    zero, one, zero, zero,
                    -sinr, zero, cosr, zero,
                    zero, zero, zero, one);
            }
            else
            {
                return Matrix4x4<T>(
                    cosr, -sinr, zero, zero,
                    sinr, cosr, zero, zero,
                    zero, zero, one, zero,
                    zero, zero, zero, one);
            }
        }

        inline constexpr static Matrix4x4<T> Inverse(const Matrix4x4<T>& m) noexcept
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

            const Vector4<T> SignA(
                static_cast<T>( 1),
                static_cast<T>(-1),
                static_cast<T>( 1),
                static_cast<T>(-1)
            );

            const Vector4<T> SignB(
                static_cast<T>(-1),
                static_cast<T>( 1),
                static_cast<T>(-1),
                static_cast<T>( 1)
            );

            const Matrix4x4<T> inverse(inv0 * SignA, inv1 * SignB, inv2 * SignA, inv3 * SignB);
            const Vector4<T> row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);
            const Vector4<T> dot0(m[0] * row0);
            const T dot1 = (dot0[0] + dot0[1]) + (dot0[2] + dot0[3]);
            const T oneOverDeterminant = static_cast<T>(1) / dot1;

            return inverse * oneOverDeterminant;
        }
    };

    template<typename T>
    class Matrix3x3 : public Matrix<3,3,T>
    {
    public:
        using Matrix<3,3,T>::Matrix;

        Matrix3x3(
                T s0, T s1,  T s2,
                T s3, T s4,  T s5,
                T s6,  T s7, T s8) noexcept
        {
            Matrix<3,3,T>::m_rows[0] = Vector4<T>(s0, s1, s2);
            Matrix<3,3,T>::m_rows[1] = Vector4<T>(s3, s4, s5);
            Matrix<3,3,T>::m_rows[2] = Vector4<T>(s6, s7, s8);
        }

        Matrix3x3(
                const Vector3<T>& row0,
                const Vector3<T>& row1,
                const Vector3<T>& row2) noexcept
        {
            Matrix<3,3,T>::m_rows[0] = row0;
            Matrix<3,3,T>::m_rows[1] = row1;
            Matrix<3,3,T>::m_rows[2] = row2;
        }

        Matrix3x3(const Matrix<3,3,T>& other) noexcept
        : Matrix<3,3,T>(other)
        {

        }

        static constexpr Matrix3x3<T> Identity() noexcept
        {
            return
            {
                1, 0, 0,
                0, 1, 0,
                0, 0, 1
            };
        }

        static constexpr Matrix3x3<T> Translate(const Vector2<T>& offset) noexcept
        {
            return
            {
                1, 0, offset.X(),
                0, 1, offset.Y(),
                0, 0, 1,
            };
        }

        static constexpr Matrix3x3<T> Scale(const Vector2<T>& scale) noexcept
        {
            return
            {
                scale.X(), 0, 0,
                0, scale.Y(), 0,
                0, 0, 1,
            };
        }

        static constexpr Matrix3x3<T> Rotate(const Vector3<T>& axis, float r) noexcept
        {
    #if defined(TML_USE_DEGREES)
            r *= 0.01745329252;
    #endif
            const auto sinr = static_cast<T>(sin(r));
            const auto cosr = static_cast<T>(cos(r));
            const auto zero = static_cast<T>(0);
            const auto one  = static_cast<T>(1);

            if(axis[0] > 0.001)
            {
                return Matrix3x3<T>(
                        Vector3<T>(one, zero, zero),
                        Vector3<T>(zero, cosr, -sinr),
                        Vector3<T>(zero, sinr, cosr));
            }
            else if(axis[1] > 0.001)
            {
                return Matrix3x3<T>(
                        Vector3<T>(cosr, zero, sinr),
                        Vector3<T>(zero, one, zero),
                        Vector3<T>(-sinr, zero, cosr));
            }
            else
            {
                return Matrix3x3<T>(
                        Vector3<T>(cosr, -sinr, zero),
                        Vector3<T>(sinr, cosr, zero),
                        Vector3<T>(zero, zero, one));
            }
        }

        static constexpr Matrix3x3<T> Inverse(const Matrix3x3<T>& m) noexcept
        {
            T oneOverDeterminant = static_cast<T>(1) / (
                    m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
                    - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
                    + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

            Matrix<3, 3, T> result;
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
    };

    template<typename T>
    class Matrix2x2 : public Matrix<2,2,T>
    {
    public:
        using Matrix<2,2,T>::Matrix;

        Matrix2x2(
                T s0, T s1,
                T s2, T s3) noexcept
        {
            Matrix<2,2,T>::m_rows[0] = Vector4<T>(s0, s1);
            Matrix<2,2,T>::m_rows[1] = Vector4<T>(s2, s3);
        }

        Matrix2x2(
                const Vector2<T>& row0,
                const Vector2<T>& row1) noexcept
        {
            Matrix<2,2,T>::m_rows[0] = row0;
            Matrix<2,2,T>::m_rows[1] = row1;
        }

        Matrix2x2(const Matrix<2,2,T>& other) noexcept
        : Matrix<2,2,T>(other)
        {

        }

        static constexpr Matrix2x2<T> Identity() noexcept
        {
            return
            {
                1, 0,
                0, 1
            };
        }

        static Matrix2x2<T> Rotate(const Vector2<T>& axis, float r) noexcept
        {
    #if defined(TML_USE_DEGREES)
            r *= 0.01745329252;
    #endif
            const auto sinr = static_cast<T>(sin(r));
            const auto cosr = static_cast<T>(cos(r));

            return Matrix2x2<T>(
                    Vector2<T>(cosr, -sinr),
                    Vector2<T>(sinr, cosr));
        }

        constexpr static Matrix2x2<T> Inverse(const Matrix2x2<T>& m) noexcept
        {
            T oneOverDeterminant = static_cast<T>(1) / (
                    m[0][0] * m[1][1]
                    -m[1][0] * m[0][1]);

            return Matrix2x2<T>(
                    m[1][1] * oneOverDeterminant,
                    -m[0][1] * oneOverDeterminant,
                    -m[1][0] * oneOverDeterminant,
                    m[0][0] * oneOverDeterminant);
        }
    };

    template<typename T>
    Matrix4x4<T> Translate(const Matrix4x4<T>& matrix, const Vector3<T>& vector) noexcept
    {
        Matrix4x4<T> result{matrix};
        result[0][3] += vector.X();
        result[1][3] += vector.Y();
        result[2][3] += vector.Z();

        return result;
    }

    template<typename T>
    Matrix4x4<T> Rotate(const Matrix4x4<T>& matrix, const Vector3<T>& vector, float angle) noexcept
    {
        return matrix * Matrix4x4<T>::Rotate(vector, angle);
    }

    template<typename T>
    Matrix4x4<T> Scale(const Matrix4x4<T>& matrix, const Vector3<T>& vector) noexcept
    {
        return matrix * Matrix4x4<T>::Scale(vector);
    }

}