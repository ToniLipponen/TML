#include <TML/Graphics/Core/Shader.h>
#include "TML/TMLAssert.h"
#include "../Headers/GLHeader.h"
#include <TML/System/File.h>
#include <iostream>
#include <array>

#define TML_INVALID_UNIFORM_LOCATION (-1)

namespace tml
{
    Shader::Shader() noexcept
    : m_id(TML_INVALID_ID)
    {

    }

    Shader::~Shader() noexcept
    {
        GL_CALL(glad_glDeleteProgram(m_id));
    }

    void Shader::Bind() const noexcept
    {
        GL_CALL(glad_glUseProgram(m_id));
    }

    void Shader::Unbind() const noexcept
    {
        GL_CALL(glad_glUseProgram(TML_INVALID_ID));
    }

    bool Shader::LoadFromFile(const std::string& vs, const std::string& fs) const noexcept
    {
        auto vert = File::GetString(vs);
        auto frag = File::GetString(fs);

        if(vert && frag)
        {
            return LoadFromString(*vert, *frag);
        }

        return false;
    }

    bool Shader::LoadFromString(const std::string& vs, const std::string& fs) const noexcept
    {
        if(m_id != TML_INVALID_ID)
        {
            GL_CALL(glad_glDeleteProgram(m_id));
        }

        m_id = GL_CALL(glad_glCreateProgram());

        uint32_t _vs = GL_CALL(glad_glCreateShader(GL_VERTEX_SHADER));
        uint32_t _fs = GL_CALL(glad_glCreateShader(GL_FRAGMENT_SHADER));
        const char* vertex   = vs.c_str();
        const char* fragment = fs.c_str();

        GL_CALL(glad_glShaderSource(_vs, 1, &vertex, nullptr));
        GL_CALL(glad_glCompileShader(_vs));

        GL_CALL(glad_glShaderSource(_fs, 1, &fragment, nullptr));
        GL_CALL(glad_glCompileShader(_fs));

        int32_t vertex_status = 1, fragment_status = 1;
        GL_CALL(glad_glGetShaderiv(_vs, GL_COMPILE_STATUS, &vertex_status));
        GL_CALL(glad_glGetShaderiv(_fs, GL_COMPILE_STATUS, &fragment_status));

        if(vertex_status != GL_TRUE)
        {
            std::array<char, 1024> message{};
            const auto readBytes = GetShaderInfoLog(_vs, message.data(), message.size());
            std::cout << "[Error]: Vertex shader error: " << std::string(message.data(), readBytes) << std::endl;
            Cleanup(_vs, _fs);
            return false;
        }

        if(fragment_status != GL_TRUE)
        {
            std::array<char, 1024> message{};
            const auto readBytes = GetShaderInfoLog(_fs, message.data(), message.size());
            std::cout << "[Error]: Fragment shader error: " << std::string(message.data(), readBytes) << std::endl;
            Cleanup(_vs, _fs);
            return false;
        }

        GL_CALL(glad_glAttachShader(m_id, _vs));
        GL_CALL(glad_glAttachShader(m_id, _fs));

        GL_CALL(glad_glLinkProgram(m_id));
        GL_CALL(glad_glValidateProgram(m_id));

        int linkStatus = 1, validationStatus = 1;

        GL_CALL(glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus));
        GL_CALL(glGetProgramiv(m_id, GL_VALIDATE_STATUS, &validationStatus));

        if(linkStatus != GL_TRUE)
        {
            std::puts("[Error]: Failed to link shader program");
            Cleanup(_vs, _fs);
            return false;
        }

        if(validationStatus != GL_TRUE)
        {
            std::puts("[Error]: Failed to validate shader program");
            Cleanup(_vs, _fs);
            return false;
        }

        Cleanup(_vs, _fs);
        return true;
    }

    inline int32_t Shader::GetUniformLocation(const std::string& name) const noexcept
    {
        if(m_uniformCache.find(name) != m_uniformCache.end())
        {
            return m_uniformCache[name];
        }

        const int32_t loc = GL_CALL(glGetUniformLocation(m_id, name.c_str()));

        if(loc != TML_INVALID_UNIFORM_LOCATION)
        {
            m_uniformCache[name] = loc;
        }

        return loc;
    }

    int32_t Shader::GetShaderInfoLog(uint32_t id, char *str, int32_t maxBytes) const noexcept
    {
        int32_t readBytes = 0;
        GL_CALL(glad_glGetShaderInfoLog(id, maxBytes, &readBytes, str));
        return readBytes;
    }

    void Shader::Uniform1f(const std::string& name, float x) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform1f(m_id, loc, x));
    }

    void Shader::Uniform2f(const std::string& name, float x, float y) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform2f(m_id, loc, x, y));
    }

    void Shader::Uniform3f(const std::string& name, float x, float y, float z) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform3f(m_id, loc, x, y, z));
    }

    void Shader::Uniform4f(const std::string& name, float x, float y, float z, float w) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform4f(m_id, loc, x, y, z, w));
    }

    void Shader::Uniform1i(const std::string& name, int32_t x) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform1i(m_id, loc, x));
    }

    void Shader::Uniform2i(const std::string& name, int32_t x, int32_t y) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform2i(m_id, loc, x, y));
    }

    void Shader::Uniform3i(const std::string& name, int32_t x, int32_t y, int32_t z) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform3i(m_id, loc, x, y, z));
    }

    void Shader::Uniform4i(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform4i(m_id, loc, x, y, z, w));
    }

    void Shader::Uniform1ui(const std::string& name, uint32_t x) const noexcept
    {
        int32_t loc = GL_CALL(GetUniformLocation(name));
        GL_CALL(GL_CALL(glad_glProgramUniform1ui(m_id, loc, x)));
    }

    void Shader::Uniform2ui(const std::string& name, uint32_t x, uint32_t y) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform2ui(m_id, loc, x, y));
    }

    void Shader::Uniform3ui(const std::string& name, uint32_t x, uint32_t y, uint32_t z) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform3ui(m_id, loc, x, y, z));
    }

    void Shader::Uniform4ui(const std::string& name, uint32_t x, uint32_t y, uint32_t z, uint32_t w) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform4ui(m_id, loc, x, y, z, w));
    }

    void Shader::Uniform1fv(const std::string& name, uint32_t count, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform1fv(m_id, loc, count, value));
    }

    void Shader::Uniform2fv(const std::string& name, uint32_t count, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform2fv(m_id, loc, count, value));
    }

    void Shader::Uniform3fv(const std::string& name, uint32_t count, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform3fv(m_id, loc, count, value));
    }

    void Shader::Uniform4fv(const std::string& name, uint32_t count, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform4fv(m_id, loc, count, value));
    }

    void Shader::Uniform1iv(const std::string& name, uint32_t count, const int32_t* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform1iv(m_id, loc, count, value));
    }

    void Shader::Uniform2iv(const std::string& name, uint32_t count, const int32_t* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform2iv(m_id, loc, count, value));
    }

    void Shader::Uniform3iv(const std::string& name, uint32_t count, const int32_t* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform3iv(m_id, loc, count, value));
    }

    void Shader::Uniform4iv(const std::string& name, uint32_t count, const int32_t* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform4iv(m_id, loc, count, value));
    }

    void Shader::Uniform1uiv(const std::string& name, uint32_t count, const uint32_t* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform1uiv(m_id, loc, count, value));
    }

    void Shader::Uniform2uiv(const std::string& name, uint32_t count, const uint32_t* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform2uiv(m_id, loc, count, value));
    }

    void Shader::Uniform3uiv(const std::string& name, uint32_t count, const uint32_t* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform3uiv(m_id, loc, count, value));
    }

    void Shader::Uniform4uiv(const std::string& name, uint32_t count, const uint32_t* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniform4uiv(m_id, loc, count, value));
    }

    void Shader::UniformMat2fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix2fv(m_id, loc, count, transpose, value));
    }

    void Shader::UniformMat3fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix3fv(m_id, loc, count, transpose, value));
    }

    void Shader::UniformMat4fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix4fv(m_id, loc, count, transpose, value));
    }

    void Shader::UniformMat2x3fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix2x3fv(m_id, loc, count, transpose, value));
    }

    void Shader::UniformMat3x2fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix3x2fv(m_id, loc, count, transpose, value));
    }

    void Shader::UniformMat2x4fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix2x4fv(m_id, loc, count, transpose, value));
    }

    void Shader::UniformMat4x2fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix4x2fv(m_id, loc, count, transpose, value));
    }

    void Shader::UniformMat3x4fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix3x4fv(m_id, loc, count, transpose, value));
    }

    void Shader::UniformMat4x3fv(const std::string& name, uint32_t count, bool transpose, const float* value) const noexcept
    {
        int32_t loc = GetUniformLocation(name);
        GL_CALL(glad_glProgramUniformMatrix4x3fv(m_id, loc, count, transpose, value));
    }

    void Shader::Cleanup(uint32_t vs, uint32_t fs) const noexcept
    {
        GL_CALL(glad_glDetachShader(m_id, vs));
        GL_CALL(glad_glDetachShader(m_id, fs));

        GL_CALL(glad_glDeleteShader(vs));
        GL_CALL(glad_glDeleteShader(fs));
    }
}