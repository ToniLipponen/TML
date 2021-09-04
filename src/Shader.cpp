#include "../include/Shader.h"
#include "../external-headers/glad/glad.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include "../include/Assert.h"
#include "../include/GlDebug.h"

using namespace tml;

constexpr float _color_mult = 1.f / 255.f;

inline void UniformError(const std::string& uniformname) noexcept
{
    std::cout << "ERROR: Could not find uniform -> " << uniformname << std::endl;
}

std::string ReadFile(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()){
        tml::Logger::ErrorMessage("Could not open shader -> %s", filename);
        return std::string("");
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
{
    m_id = glCreateProgram();
}

Shader::Shader(cstring vs, cstring fs, cstring gs)
{
    Load(vs, fs, gs);
}

void Shader::Bind() const 
{
    glUseProgram(m_id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::Load(cstring vs, cstring fs, cstring gs) const
{
    std::string vert = ReadFile(vs);
    std::string frag = ReadFile(fs);
    FromString(vert, frag);
}

void Shader::FromString(const std::string& vs, const std::string& fs) const
{
    ui32 _vs = glCreateShader(GL_VERTEX_SHADER);
    ui32 _fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char* vertex   = vs.c_str();
    const char* fragment = fs.c_str();

    glShaderSource(_vs, 1, &vertex, nullptr);
    glCompileShader(_vs);

    glShaderSource(_fs, 1, &fragment, nullptr);
    glCompileShader(_fs);

    i32 vertex_status = 1, fragment_status = 1, geometry_status = 1;
    glGetShaderiv(_vs, GL_COMPILE_STATUS, &vertex_status);
    glGetShaderiv(_fs, GL_COMPILE_STATUS, &fragment_status);

    if(vertex_status != 1){
        char vertex_message[1024*4];
        i32 vertex_message_len = 0;
        glGetShaderInfoLog(_vs, 500, &vertex_message_len, vertex_message);
        tml::Logger::ErrorMessage("Vertex shader error at %s", vertex_message);
    }
    if(fragment_status != 1){
        char fragment_message[1024*4];
        i32 fragment_message_len = 0;
        glGetShaderInfoLog(_fs, 500, &fragment_message_len, fragment_message);
        tml::Logger::ErrorMessage("Fragment shader error at %s", fragment_message);
    }

    glAttachShader(m_id, _vs);
    glAttachShader(m_id, _fs);

    glLinkProgram(m_id);
    glValidateProgram(m_id);

    glDetachShader(m_id, _vs);
    glDetachShader(m_id, _fs);
    glDeleteShader(_vs);
    glDeleteShader(_fs);
}

inline i32 Shader::GetUniformLocation(const std::string& name) const
{
    if(m_uniform_cache.count(name) != 0)
        return m_uniform_cache[name];
    i32 loc = glGetUniformLocation(m_id, name.c_str());
    if(loc != -1)
        m_uniform_cache[name] = loc;
    return loc;
}
#ifndef TML_GL_VERSION_330
void Shader::Uniform1f(const std::string& name, float x) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform1f(m_id, loc, x);
}
void Shader::Uniform2f(const std::string& name, float x, float y) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform2f(m_id, loc, x, y);
}
void Shader::Uniform3f(const std::string& name, float x, float y, float z) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform3f(m_id, loc, x, y, z);
}
void Shader::Uniform4f(const std::string& name, float x, float y, float z, float w) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform4f(m_id, loc, x, y, z, w);
}

void Shader::Uniform1i(const std::string& name, i32 x) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform1i(m_id, loc, x);
}
void Shader::Uniform2i(const std::string& name, i32 x, i32 y) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform2i(m_id, loc, x, y);
}
void Shader::Uniform3i(const std::string& name, i32 x, i32 y, i32 z) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform3i(m_id, loc, x, y, z);
}
void Shader::Uniform4i(const std::string& name, i32 x, i32 y, i32 z, i32 w) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform4i(m_id, loc, x, y, z, w);
}

void Shader::Uniform1ui(const std::string& name, ui32 x) const
{
    i32 loc = GL_CALL(GetUniformLocation(name));
    if(loc == -1) return;
    GL_CALL(glProgramUniform1ui(m_id, loc, x));
}
void Shader::Uniform2ui(const std::string& name, ui32 x, ui32 y) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform2ui(m_id, loc, x, y);
}
void Shader::Uniform3ui(const std::string& name, ui32 x, ui32 y, ui32 z) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform3ui(m_id, loc, x, y, z);
}
void Shader::Uniform4ui(const std::string& name, ui32 x, ui32 y, ui32 z, ui32 w) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform4ui(m_id, loc, x, y, z, w);
}
void Shader::Uniform1fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform1fv(m_id, loc, count, value);
}
void Shader::Uniform2fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform2fv(m_id, loc, count, value);
}
void Shader::Uniform3fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform3fv(m_id, loc, count, value);
}
void Shader::Uniform4fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform4fv(m_id, loc, count, value);
}
void Shader::Uniform1iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform1iv(m_id, loc, count, value);
}
void Shader::Uniform2iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform2iv(m_id, loc, count, value);
}
void Shader::Uniform3iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform3iv(m_id, loc, count, value);
}
void Shader::Uniform4iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform4iv(m_id, loc, count, value);
}
void Shader::Uniform1uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform1uiv(m_id, loc, count, value);
}
void Shader::Uniform2uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform2uiv(m_id, loc, count, value);
}
void Shader::Uniform3uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform3uiv(m_id, loc, count, value);
}
void Shader::Uniform4uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniform4uiv(m_id, loc, count, value);
}
void Shader::UniformMat2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix2fv(m_id, loc, count, transpose, value);
}
void Shader::UniformMat3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix3fv(m_id, loc, count, transpose, value);
}
void Shader::UniformMat4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix4fv(m_id, loc, count, transpose, value);
}
void Shader::UniformMat2x3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix2x3fv(m_id, loc, count, transpose, value);
}
void Shader::UniformMat3x2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix3x2fv(m_id, loc, count, transpose, value);
}
void Shader::UniformMat2x4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix2x4fv(m_id, loc, count, transpose, value);
}
void Shader::UniformMat4x2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix4x2fv(m_id, loc, count, transpose, value);
}
void Shader::UniformMat3x4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix3x4fv(m_id, loc, count, transpose, value);
}
void Shader::UniformMat4x3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glProgramUniformMatrix4x3fv(m_id, loc, count, transpose, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    int loc = GetUniformLocation(name);
    glProgramUniform1f(m_id, loc, value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    int loc = GetUniformLocation(name);
    glProgramUniform1i(m_id, loc, value);
}

void Shader::SetVec2(const std::string &name, const Vector2 &vector) const
{
   int loc = GetUniformLocation(name);
   glUniform2f(loc, vector.x, vector.y);
}
#else
void Shader::Uniform1f(const std::string& name, float x) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform1f(loc, x);
}
void Shader::Uniform2f(const std::string& name, float x, float y) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform2f(loc, x, y);
}
void Shader::Uniform3f(const std::string& name, float x, float y, float z) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform3f(loc, x, y, z);
}
void Shader::Uniform4f(const std::string& name, float x, float y, float z, float w) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform4f(loc, x, y, z, w);
}

void Shader::Uniform1i(const std::string& name, i32 x) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform1i(loc, x);
}
void Shader::Uniform2i(const std::string& name, i32 x, i32 y) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform2i(loc, x, y);
}
void Shader::Uniform3i(const std::string& name, i32 x, i32 y, i32 z) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform3i(loc, x, y, z);
}
void Shader::Uniform4i(const std::string& name, i32 x, i32 y, i32 z, i32 w) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform4i(loc, x, y, z, w);
}

void Shader::Uniform1ui(const std::string& name, ui32 x) const
{
    i32 loc = GL_CALL(GetUniformLocation(name));
    if(loc == -1) return;
    GL_CALL(glad_glUniform1ui(loc, x));
}
void Shader::Uniform2ui(const std::string& name, ui32 x, ui32 y) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform2ui(loc, x, y);
}
void Shader::Uniform3ui(const std::string& name, ui32 x, ui32 y, ui32 z) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform3ui(loc, x, y, z);
}
void Shader::Uniform4ui(const std::string& name, ui32 x, ui32 y, ui32 z, ui32 w) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform4ui(loc, x, y, z, w);
}
void Shader::Uniform1fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform1fv(loc, count, value);
}
void Shader::Uniform2fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform2fv(loc, count, value);
}
void Shader::Uniform3fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform3fv(loc, count, value);
}
void Shader::Uniform4fv(const std::string& name, ui32 count, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform4fv(loc, count, value);
}
void Shader::Uniform1iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform1iv(loc, count, value);
}
void Shader::Uniform2iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform2iv(loc, count, value);
}
void Shader::Uniform3iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform3iv(loc, count, value);
}
void Shader::Uniform4iv(const std::string& name, ui32 count, const i32* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform4iv(loc, count, value);
}
void Shader::Uniform1uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform1uiv(loc, count, value);
}
void Shader::Uniform2uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform2uiv(loc, count, value);
}
void Shader::Uniform3uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform3uiv(loc, count, value);
}
void Shader::Uniform4uiv(const std::string& name, ui32 count, const ui32* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniform4uiv(loc, count, value);
}
void Shader::UniformMat2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix2fv(loc, count, transpose, value);
}
void Shader::UniformMat3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix3fv(loc, count, transpose, value);
}
void Shader::UniformMat4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix4fv(loc, count, transpose, value);
}
void Shader::UniformMat2x3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix2x3fv(loc, count, transpose, value);
}
void Shader::UniformMat3x2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix3x2fv(loc, count, transpose, value);
}
void Shader::UniformMat2x4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix2x4fv(loc, count, transpose, value);
}
void Shader::UniformMat4x2fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix4x2fv(loc, count, transpose, value);
}
void Shader::UniformMat3x4fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix3x4fv(loc, count, transpose, value);
}
void Shader::UniformMat4x3fv(const std::string& name, ui32 count, bool transpose, const float* value) const
{
    i32 loc = GetUniformLocation(name);
    glad_glUniformMatrix4x3fv(loc, count, transpose, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    int loc = GetUniformLocation(name);
    glad_glUniform1f(loc, value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    int loc = GetUniformLocation(name);
    glad_glUniform1i(loc, value);
}

void Shader::SetVec2(const std::string &name, const Vector2 &vector) const
{
   int loc = GetUniformLocation(name);
   glad_glUniform2f(loc, vector.x, vector.y);
}
#endif
// void Shader::SetVec3(const std::string &name, const glm::vec3 &vector) const
// {
//    int loc = GetUniformLocation(name);
//    glUniform3f(loc, vector.x, vector.y, vector.z);
// }