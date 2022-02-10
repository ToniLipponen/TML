#include "TML/Graphics/Core/ComputeShader.h"
#include "TML/System/File.h"
#include "Headers/GlDebug.h"
#include "Headers/GLHeader.h"

namespace tml
{
    void ComputeShader::LoadFromFile(const String &filename) noexcept
    {
        InFile file;
        file.Open(filename.cpp_str());
        auto src = file.GetString();

        LoadFromString(src);
    }

    void ComputeShader::LoadFromString(const String &source) noexcept
    {
        if(m_id == 0)
            m_id = GL_CALL(glad_glCreateProgram());

        ui32 shaderID = GL_CALL(glad_glCreateShader(GL_COMPUTE_SHADER));

        const char* src = source.c_str();
        GL_CALL(glad_glShaderSource(shaderID, 1, &src, nullptr));
        GL_CALL(glad_glCompileShader(shaderID));

        i32 compileStatus = 1;
        GL_CALL(glad_glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus));

        if(compileStatus != GL_TRUE)
        {
            char vertex_message[1024];
            i32 vertex_message_len = 0;
            GL_CALL(glad_glGetShaderInfoLog(shaderID, 1024, &vertex_message_len, vertex_message));
            tml::Logger::ErrorMessage("Compute shader error at %s", vertex_message);
        }

        GL_CALL(glad_glAttachShader(m_id, shaderID));
        GL_CALL(glad_glLinkProgram(m_id));
        GL_CALL(glad_glValidateProgram(m_id));

        int linkStatus = 1, validationStatus = 1;
        GL_CALL(glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus));
        GL_CALL(glGetProgramiv(m_id, GL_VALIDATE_STATUS, &validationStatus));

        if(linkStatus != GL_TRUE)
            Logger::ErrorMessage("Failed to link shader program");

        if(validationStatus != GL_TRUE)
            Logger::ErrorMessage("Failed to validate shader program");

        GL_CALL(glad_glDetachShader(m_id, shaderID));
        GL_CALL(glad_glDeleteShader(shaderID));
    }

    void ComputeShader::ConnectBuffer(const String &name, ui32 index)
    {
        const ui32 block_index = GL_CALL(glGetProgramResourceIndex(m_id, GL_SHADER_STORAGE_BLOCK, name.c_str()));
        GL_CALL(glShaderStorageBlockBinding(m_id, block_index, index));
    }

    void ComputeShader::Dispatch(i16 x, i16 y)
    {
        GL_CALL(glad_glDispatchCompute(x,y,1));
    }

    void ComputeShader::Wait()
    {
        GL_CALL(glad_glMemoryBarrier(GL_ALL_BARRIER_BITS));
    }
}