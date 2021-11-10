#include "internal/Assert.h"
#include "internal/GlDebug.h"
#include "internal/Shader.h"
#include <GLHeader.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace tml;

std::string ReadFile(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()){
        tml::Logger::ErrorMessage("Could not open shader -> %s", filename);
        return "";
    }
    std::string line, fileContents;
    while(std::getline(file, line))
    {
        fileContents += line + "\n";
    }
    file.close();
    return fileContents;
}

Shader::Shader()
: m_id(-1)
{
    m_id = GL_CALL(glad_glCreateProgram());
}

Shader::Shader(cstring vs, cstring fs)
: m_id(-1)
{
    m_id = GL_CALL(glad_glCreateProgram());
    Load(vs, fs);
}

void Shader::Bind() const 
{
    GL_CALL(glad_glUseProgram(m_id));
}

void Shader::Unbind() const
{
    GL_CALL(glad_glUseProgram(0));
}

void Shader::Load(cstring vs, cstring fs) const
{
    std::string vert = ReadFile(vs);
    std::string frag = ReadFile(fs);
    FromString(vert, frag);
}

void Shader::FromString(const std::string& vs, const std::string& fs) const
{
    ui32 _vs = GL_CALL(glad_glCreateShader(GL_VERTEX_SHADER));
    ui32 _fs = GL_CALL(glad_glCreateShader(GL_FRAGMENT_SHADER));
    const char* vertex   = vs.c_str();
    const char* fragment = fs.c_str();

    GL_CALL(glad_glShaderSource(_vs, 1, &vertex, nullptr));
    GL_CALL(glad_glCompileShader(_vs));

    GL_CALL(glad_glShaderSource(_fs, 1, &fragment, nullptr));
    GL_CALL(glad_glCompileShader(_fs));

    i32 vertex_status = 1, fragment_status = 1;
    GL_CALL(glad_glGetShaderiv(_vs, GL_COMPILE_STATUS, &vertex_status));
    GL_CALL(glad_glGetShaderiv(_fs, GL_COMPILE_STATUS, &fragment_status));

    if(vertex_status != 1){
        char vertex_message[1024*4];
        i32 vertex_message_len = 0;
        GL_CALL(glad_glGetShaderInfoLog(_vs, 500, &vertex_message_len, vertex_message));
        tml::Logger::ErrorMessage("Vertex shader error at %s", vertex_message);
    }
    if(fragment_status != 1){
        char fragment_message[1024*4];
        i32 fragment_message_len = 0;
        GL_CALL(glad_glGetShaderInfoLog(_fs, 500, &fragment_message_len, fragment_message));
        tml::Logger::ErrorMessage("Fragment shader error at %s", fragment_message);
    }

    GL_CALL(glad_glAttachShader(m_id, _vs));
    GL_CALL(glad_glAttachShader(m_id, _fs));

    GL_CALL(glad_glLinkProgram(m_id));
    GL_CALL(glad_glValidateProgram(m_id));

    GL_CALL(glad_glDetachShader(m_id, _vs));
    GL_CALL(glad_glDetachShader(m_id, _fs));
    GL_CALL(glad_glDeleteShader(_vs));
    GL_CALL(glad_glDeleteShader(_fs));
}

inline i32 Shader::GetUniformLocation(const std::string& name) const
{
    if(m_uniform_cache.find(name) != m_uniform_cache.end())
        return m_uniform_cache[name];

    i32 loc = GL_CALL(glGetUniformLocation(m_id, name.c_str()));
    if(loc != -1)
        m_uniform_cache[name] = loc;
    return loc;
}

void Shader::Uniform1f(const std::string& name, float x) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform1f(m_id, loc, x));
}
void Shader::Uniform2f(const std::string& name, float x, float y) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform2f(m_id, loc, x, y));
}
void Shader::Uniform3f(const std::string& name, float x, float y, float z) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform3f(m_id, loc, x, y, z));
}
void Shader::Uniform4f(const std::string& name, float x, float y, float z, float w) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform4f(m_id, loc, x, y, z, w));
}

void Shader::Uniform1i(const std::string& name, i32 x) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform1i(m_id, loc, x));
}
void Shader::Uniform2i(const std::string& name, i32 x, i32 y) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform2i(m_id, loc, x, y));
}
void Shader::Uniform3i(const std::string& name, i32 x, i32 y, i32 z) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform3i(m_id, loc, x, y, z));
}
void Shader::Uniform4i(const std::string& name, i32 x, i32 y, i32 z, i32 w) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform4i(m_id, loc, x, y, z, w));
}

void Shader::Uniform1ui(const std::string& name, ui32 x) const
{
    i32 loc = GL_CALL(GetUniformLocation(name));
    GL_CALL(GL_CALL(glad_glProgramUniform1ui(m_id, loc, x)));
}
void Shader::Uniform2ui(const std::string& name, ui32 x, ui32 y) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform2ui(m_id, loc, x, y));
}
void Shader::Uniform3ui(const std::string& name, ui32 x, ui32 y, ui32 z) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform3ui(m_id, loc, x, y, z));
}
void Shader::Uniform4ui(const std::string& name, ui32 x, ui32 y, ui32 z, ui32 w) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform4ui(m_id, loc, x, y, z, w));
}
void Shader::Uniform1fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform1fv(m_id, loc, count, value));
}
void Shader::Uniform2fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform2fv(m_id, loc, count, value));
}
void Shader::Uniform3fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform3fv(m_id, loc, count, value));
}
void Shader::Uniform4fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform4fv(m_id, loc, count, value));
}
void Shader::Uniform1iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform1iv(m_id, loc, count, value));
}
void Shader::Uniform2iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform2iv(m_id, loc, count, value));
}
void Shader::Uniform3iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform3iv(m_id, loc, count, value));
}
void Shader::Uniform4iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform4iv(m_id, loc, count, value));
}
void Shader::Uniform1uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform1uiv(m_id, loc, count, value));
}
void Shader::Uniform2uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform2uiv(m_id, loc, count, value));
}
void Shader::Uniform3uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform3uiv(m_id, loc, count, value));
}
void Shader::Uniform4uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform4uiv(m_id, loc, count, value));
}
void Shader::UniformMat2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix2fv(m_id, loc, count, transpose, value));
}
void Shader::UniformMat3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix3fv(m_id, loc, count, transpose, value));
}
void Shader::UniformMat4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix4fv(m_id, loc, count, transpose, value));
}
void Shader::UniformMat2x3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix2x3fv(m_id, loc, count, transpose, value));
}
void Shader::UniformMat3x2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix3x2fv(m_id, loc, count, transpose, value));
}
void Shader::UniformMat2x4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix2x4fv(m_id, loc, count, transpose, value));
}
void Shader::UniformMat4x2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix4x2fv(m_id, loc, count, transpose, value));
}
void Shader::UniformMat3x4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix3x4fv(m_id, loc, count, transpose, value));
}
void Shader::UniformMat4x3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniformMatrix4x3fv(m_id, loc, count, transpose, value));
}

void Shader::SetFloat(const std::string& name, float value) const
{
    int loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform1f(m_id, loc, value));
}

void Shader::SetInt(const std::string &name, int value) const
{
    int loc = GetUniformLocation(name);
    GL_CALL(glad_glProgramUniform1i(m_id, loc, value));
}

void Shader::SetVec2(const std::string &name, const Vector2 &vector) const
{
   int loc = GetUniformLocation(name);
   GL_CALL(glad_glUniform2f(loc, vector.x, vector.y));
}