#pragma once
#include "TML/Utilities/Types.h"
#include <string>
#include <unordered_map>

namespace tml {
    class Shader {
    public:
        Shader();
        Shader(cstring vs, cstring fs);
        void Bind() const;
        void Unbind() const;
        void Load(cstring vs, cstring fs) const;
        void FromString(const std::string &vs, const std::string &fs) const;

    public:
        void Uniform1f(const std::string &, f32 x) const;
        void Uniform2f(const std::string &, f32 x, f32 y) const;
        void Uniform3f(const std::string &, f32 x, f32 y, f32 z) const;
        void Uniform4f(const std::string &, f32 x, f32 y, f32 z, f32 w) const;
        void Uniform1i(const std::string &, i32 x) const;
        void Uniform2i(const std::string &, i32 x, i32 y) const;
        void Uniform3i(const std::string &, i32 x, i32 y, i32 z) const;
        void Uniform4i(const std::string &, i32 x, i32 y, i32 z, i32 w) const;
        void Uniform1ui(const std::string &, ui32 x) const;
        void Uniform2ui(const std::string &, ui32 x, ui32 y) const;
        void Uniform3ui(const std::string &, ui32 x, ui32 y, ui32 z) const;
        void Uniform4ui(const std::string &, ui32 x, ui32 y, ui32 z, ui32 w) const;
        void Uniform1fv(const std::string &, ui32 count, const f32 *value) const;
        void Uniform2fv(const std::string &, ui32 count, const f32 *value) const;
        void Uniform3fv(const std::string &, ui32 count, const f32 *value) const;
        void Uniform4fv(const std::string &, ui32 count, const f32 *value) const;
        void Uniform1iv(const std::string &, ui32 count, const i32 *value) const;
        void Uniform2iv(const std::string &, ui32 count, const i32 *value) const;
        void Uniform3iv(const std::string &, ui32 count, const i32 *value) const;
        void Uniform4iv(const std::string &, ui32 count, const i32 *value) const;
        void Uniform1uiv(const std::string &, ui32 count, const ui32 *value) const;
        void Uniform2uiv(const std::string &, ui32 count, const ui32 *value) const;
        void Uniform3uiv(const std::string &, ui32 count, const ui32 *value) const;
        void Uniform4uiv(const std::string &, ui32 count, const ui32 *value) const;
        void UniformMat2fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;
        void UniformMat3fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;
        void UniformMat4fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;
        void UniformMat2x3fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;
        void UniformMat2x4fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;
        void UniformMat3x2fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;
        void UniformMat3x4fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;
        void UniformMat4x2fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;
        void UniformMat4x3fv(const std::string &, ui32 count, bool transpose, const f32 *value) const;

    public:
        void SetFloat(const std::string &name, f32 value) const;
        void SetInt(const std::string &name, i32 value) const;
        void SetVec2(const std::string &name, const Vector2 &vector) const;

    private:
        inline i32 GetUniformLocation(const std::string &name) const;

    private:
        mutable ui32 m_id;
        mutable std::unordered_map<std::string, i32> m_uniform_cache;
    };
};