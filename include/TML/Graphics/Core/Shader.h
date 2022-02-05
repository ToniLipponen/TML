#pragma once
#include <TML/Utilities/Types.h>
#include <unordered_map>
#include <string>

namespace tml
{
    class Shader
    {
    public:
        Shader();
        ~Shader();

        void Bind() const noexcept;
        void Unbind() const noexcept;
        void Load(const std::string& vs, const std::string& fs) const noexcept;
        void FromString(const std::string& vs, const std::string& fs) const noexcept;

    public:
        void Uniform1f(const std::string &, f32 x) const noexcept;
        void Uniform2f(const std::string &, f32 x, f32 y) const noexcept;
        void Uniform3f(const std::string &, f32 x, f32 y, f32 z) const noexcept;
        void Uniform4f(const std::string &, f32 x, f32 y, f32 z, f32 w) const noexcept;
        void Uniform1i(const std::string &, i32 x) const noexcept;
        void Uniform2i(const std::string &, i32 x, i32 y) const noexcept;
        void Uniform3i(const std::string &, i32 x, i32 y, i32 z) const noexcept;
        void Uniform4i(const std::string &, i32 x, i32 y, i32 z, i32 w) const noexcept;
        void Uniform1ui(const std::string &, ui32 x) const noexcept;
        void Uniform2ui(const std::string &, ui32 x, ui32 y) const noexcept;
        void Uniform3ui(const std::string &, ui32 x, ui32 y, ui32 z) const noexcept;
        void Uniform4ui(const std::string &, ui32 x, ui32 y, ui32 z, ui32 w) const noexcept;
        void Uniform1fv(const std::string &, ui32 count, const f32 *value) const noexcept;
        void Uniform2fv(const std::string &, ui32 count, const f32 *value) const noexcept;
        void Uniform3fv(const std::string &, ui32 count, const f32 *value) const noexcept;
        void Uniform4fv(const std::string &, ui32 count, const f32 *value) const noexcept;
        void Uniform1iv(const std::string &, ui32 count, const i32 *value) const noexcept;
        void Uniform2iv(const std::string &, ui32 count, const i32 *value) const noexcept;
        void Uniform3iv(const std::string &, ui32 count, const i32 *value) const noexcept;
        void Uniform4iv(const std::string &, ui32 count, const i32 *value) const noexcept;
        void Uniform1uiv(const std::string &, ui32 count, const ui32 *value) const noexcept;
        void Uniform2uiv(const std::string &, ui32 count, const ui32 *value) const noexcept;
        void Uniform3uiv(const std::string &, ui32 count, const ui32 *value) const noexcept;
        void Uniform4uiv(const std::string &, ui32 count, const ui32 *value) const noexcept;
        void UniformMat2fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;
        void UniformMat3fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;
        void UniformMat4fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;
        void UniformMat2x3fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;
        void UniformMat2x4fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;
        void UniformMat3x2fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;
        void UniformMat3x4fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;
        void UniformMat4x2fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;
        void UniformMat4x3fv(const std::string &, ui32 count, bool transpose, const f32 *value) const noexcept;

    public:
        void SetFloat(const std::string &name, f32 value) const noexcept;
        void SetInt(const std::string &name, i32 value) const noexcept;
        void SetVec2(const std::string &name, const Vector2f &vector) const noexcept;

    private:
        inline i32 GetUniformLocation(const std::string &name) const noexcept;

    private:
        mutable ui32 m_id = 0;
        mutable std::unordered_map<std::string, i32> m_uniform_cache;
    };
};