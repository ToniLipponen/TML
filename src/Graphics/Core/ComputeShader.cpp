#include <TML/Graphics/Core/ComputeShader.h>
#include <TML/System/File.h>
#include "../../Headers/GLHeader.h"

#ifndef TML_USE_GLES
namespace tml
{
    ComputeShader::ComputeShader() = default;

    bool ComputeShader::LoadFromFile(const String &filename) noexcept
    {
        InFile file;
        file.Open(filename.cpp_str());
        auto src = file.GetString();
        return LoadFromString(src);
    }

    bool ComputeShader::LoadFromString(const String &source) noexcept
    {
        if(m_id == 0)
            m_id = GL_CALL(glad_glCreateProgram());

        uint32_t shaderID = GL_CALL(glad_glCreateShader(GL_COMPUTE_SHADER));

        const char* src = source.c_str();
        GL_CALL(glad_glShaderSource(shaderID, 1, &src, nullptr));
        GL_CALL(glad_glCompileShader(shaderID));

        int32_t compileStatus = 1;
        GL_CALL(glad_glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus));

        if(compileStatus != GL_TRUE)
        {
            char vertex_message[1024];
            int32_t vertex_message_len = 0;
            GL_CALL(glad_glGetShaderInfoLog(shaderID, 1024, &vertex_message_len, vertex_message));
            tml::Logger::ErrorMessage("Compute shader error at %s", vertex_message);
            return false;
        }

        GL_CALL(glad_glAttachShader(m_id, shaderID));
        GL_CALL(glad_glLinkProgram(m_id));
        GL_CALL(glad_glValidateProgram(m_id));

        int linkStatus = 1, validationStatus = 1;
        GL_CALL(glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus));
        GL_CALL(glGetProgramiv(m_id, GL_VALIDATE_STATUS, &validationStatus));

        if(linkStatus != GL_TRUE)
        {
            Logger::ErrorMessage("Failed to link shader program");
            return false;
        }

        if(validationStatus != GL_TRUE)
        {
            Logger::ErrorMessage("Failed to validate shader program");
            return false;
        }

        GL_CALL(glad_glDetachShader(m_id, shaderID));
        GL_CALL(glad_glDeleteShader(shaderID));
        return true;
    }

    void ComputeShader::ConnectBuffer(const std::string &name, uint32_t index, StorageBuffer& buffer) noexcept
    {
        const uint32_t blockIndex = GetResourceIndex(name);
        GL_CALL(glad_glShaderStorageBlockBinding(m_id, blockIndex, index));
        buffer.BindBufferBase(index);
    }

    void ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z) noexcept
    {
        GL_CALL(glad_glDispatchCompute(x,y,z));
    }

    void ComputeShader::Wait() noexcept
    {
        GL_CALL(glad_glMemoryBarrier(GL_ALL_BARRIER_BITS));
    }

    inline uint32_t ComputeShader::GetResourceIndex(const std::string& name) noexcept
    {
        if(m_resourceIndexCache.find(name) != m_resourceIndexCache.end())
            return m_resourceIndexCache.at(name);

        const uint32_t index = GL_CALL(glad_glGetProgramResourceIndex(m_id, GL_SHADER_STORAGE_BLOCK, name.c_str()));
        m_resourceIndexCache[name] = index;
        return index;
    }
}

#else
namespace tml
{
    ComputeShader::ComputeShader()
    {
        Logger::ErrorMessage("Compute shaders are not supported on OpenGL ES.");
        exit(80085);
    }

    bool ComputeShader::LoadFromFile(const String &filename) noexcept
    {
        Logger::ErrorMessage("Compute shaders are not supported on OpenGL ES.");
        exit(80085);
        return false;
    }

    bool ComputeShader::LoadFromString(const String &source) noexcept
    {
        Logger::ErrorMessage("Compute shaders are not supported on OpenGL ES.");
        exit(80085);
        return false;
    }

    void ComputeShader::ConnectBuffer(const std::string &name, uint32_t index, StorageBuffer& buffer) noexcept
    {
        Logger::ErrorMessage("Compute shaders are not supported on OpenGL ES.");
        exit(80085);
    }

    void ComputeShader::Dispatch(uint32_t x, uint32_t y, uint32_t z) noexcept
    {
        Logger::ErrorMessage("Compute shaders are not supported on OpenGL ES.");
        exit(80085);
    }

    void ComputeShader::Wait() noexcept
    {
        Logger::ErrorMessage("Compute shaders are not supported on OpenGL ES.");
        exit(80085);
    }

    inline uint32_t ComputeShader::GetResourceIndex(const std::string& name) noexcept
    {
        Logger::ErrorMessage("Compute shaders are not supported on OpenGL ES.");
        exit(80085);
    }
}
#endif