#pragma once
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
        void LoadFromFile(const std::string& vs, const std::string& fs) const noexcept;
        void LoadFromString(const std::string& vs, const std::string& fs) const noexcept;

        void Uniform1f(const std::string&, float x) const noexcept;
        void Uniform2f(const std::string&, float x, float y) const noexcept;
        void Uniform3f(const std::string&, float x, float y, float z) const noexcept;
        void Uniform4f(const std::string&, float x, float y, float z, float w) const noexcept;
        void Uniform1i(const std::string&, int32_t x) const noexcept;
        void Uniform2i(const std::string&, int32_t x, int32_t y) const noexcept;
        void Uniform3i(const std::string&, int32_t x, int32_t y, int32_t z) const noexcept;
        void Uniform4i(const std::string&, int32_t x, int32_t y, int32_t z, int32_t w) const noexcept;
        void Uniform1ui(const std::string&, uint32_t x) const noexcept;
        void Uniform2ui(const std::string&, uint32_t x, uint32_t y) const noexcept;
        void Uniform3ui(const std::string&, uint32_t x, uint32_t y, uint32_t z) const noexcept;
        void Uniform4ui(const std::string&, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const noexcept;
        void Uniform1fv(const std::string&, uint32_t count, const float *value) const noexcept;
        void Uniform2fv(const std::string&, uint32_t count, const float *value) const noexcept;
        void Uniform3fv(const std::string&, uint32_t count, const float *value) const noexcept;
        void Uniform4fv(const std::string&, uint32_t count, const float *value) const noexcept;
        void Uniform1iv(const std::string&, uint32_t count, const int32_t *value) const noexcept;
        void Uniform2iv(const std::string&, uint32_t count, const int32_t *value) const noexcept;
        void Uniform3iv(const std::string&, uint32_t count, const int32_t *value) const noexcept;
        void Uniform4iv(const std::string&, uint32_t count, const int32_t *value) const noexcept;
        void Uniform1uiv(const std::string&, uint32_t count, const uint32_t *value) const noexcept;
        void Uniform2uiv(const std::string&, uint32_t count, const uint32_t *value) const noexcept;
        void Uniform3uiv(const std::string&, uint32_t count, const uint32_t *value) const noexcept;
        void Uniform4uiv(const std::string&, uint32_t count, const uint32_t *value) const noexcept;
        void UniformMat2fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;
        void UniformMat3fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;
        void UniformMat4fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;
        void UniformMat2x3fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;
        void UniformMat2x4fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;
        void UniformMat3x2fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;
        void UniformMat3x4fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;
        void UniformMat4x2fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;
        void UniformMat4x3fv(const std::string&, uint32_t count, bool transpose, const float *value) const noexcept;

    protected:
        inline int32_t GetUniformLocation(const std::string& name) const noexcept;

    protected:
        mutable std::unordered_map<std::string, int32_t> m_uniformCache;
        mutable uint32_t m_id = 0;
    };
};