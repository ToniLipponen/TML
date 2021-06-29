#include "../include/Shader.h"
#include "../include/glad/glad.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include "../include/Assert.h"

constexpr float _color_mult = 1.f / 255.f;

inline void UniformError(const std::string& uniformname) noexcept
{
    std::cout << "ERROR: Could not find uniform -> " << uniformname << std::endl;
}

std::string ReadFile(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()){
        tl::Logger::ErrorMessage("Could not open shader -> %s", filename);
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
    std::string geo;
    if(gs[0] != '\0')
        geo = ReadFile(gs);
    
    this->m_id = glCreateProgram();
    ui32 _vs = glCreateShader(GL_VERTEX_SHADER);
    ui32 _fs = glCreateShader(GL_FRAGMENT_SHADER);
    ui32 _gs = glCreateShader(GL_GEOMETRY_SHADER);

    const char* vertex   = vert.c_str();
    const char* fragment = frag.c_str();
    const char* geometry = geo.c_str();
    const bool has_geo = (geo.length() > 1);

    glShaderSource(_vs, 1, &vertex, nullptr);
    glCompileShader(_vs);

    glShaderSource(_fs, 1, &fragment, nullptr);
    glCompileShader(_fs);

    if(has_geo)
    {
        glShaderSource(_gs, 1, &geometry, nullptr);
        glCompileShader(_gs);
    }

    i32 vertex_status = 1, fragment_status = 1, geometry_status = 1;
    glGetShaderiv(_vs, GL_COMPILE_STATUS, &vertex_status);
    glGetShaderiv(_fs, GL_COMPILE_STATUS, &fragment_status);
    if(has_geo)
        glGetShaderiv(_gs, GL_COMPILE_STATUS, &geometry_status);
    
    if(vertex_status != 1){
        char vertex_message[1024*4];
        i32 vertex_message_len = 0;
        glGetShaderInfoLog(_vs, 500, &vertex_message_len, vertex_message);
        tl::Logger::ErrorMessage(vertex_message);
        assert(vertex_status == 1);
    }
    if(fragment_status != 1){
        char fragment_message[1024*4];
        i32 fragment_message_len = 0;
        glGetShaderInfoLog(_fs, 500, &fragment_message_len, fragment_message);
        tl::Logger::ErrorMessage(fragment_message);
        assert(fragment_status == 1);
    }
    if(geometry_status != 1){
        char message[1024*4];
        i32 message_len = 0;
        glGetShaderInfoLog(_fs, 500, &message_len, message);
        tl::Logger::ErrorMessage(message);
        assert(geometry_status == 1);
    }

    glAttachShader(m_id, _vs);
    glAttachShader(m_id, _fs);
    if(has_geo)
        glAttachShader(m_id, _gs);


    glLinkProgram(m_id);
    glValidateProgram(m_id);

    glDetachShader(m_id, _vs);
    glDetachShader(m_id, _fs);
    if(has_geo)
        glAttachShader(m_id, _gs);

    glDeleteShader(_vs);
    glDeleteShader(_fs);
    if(has_geo)
        glDeleteShader(_gs);
}

void Shader::FromString(const std::string& fs, const std::string& vs) const
{
    this->m_id = glCreateProgram();
    ui32 _vs = glCreateShader(GL_VERTEX_SHADER);
    ui32 _fs = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertex   = vs.c_str();
    const char* fragment = fs.c_str();

    glShaderSource(_vs, 1, &vertex, nullptr);
    glCompileShader(_vs);
    glShaderSource(_fs, 1, &fragment, nullptr);
    glCompileShader(_fs);

    i32 vertex_status = 1, fragment_status = 1;

    glGetShaderiv(_vs, GL_COMPILE_STATUS, &vertex_status);
    glGetShaderiv(_fs, GL_COMPILE_STATUS, &fragment_status);
    
    if(vertex_status != 1){
        std::printf("ERROR: Failed to compile vertex shader -> %s\n", vs.c_str());
        assert(vertex_status == 1);
    }
    if(fragment_status != 1){
        std::printf("ERROR: Failed to compile fragment shader -> %s\n", fs.c_str());
        assert(fragment_status == 1);
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
    i32 loc = GetUniformLocation(name);
    glProgramUniform1ui(m_id, loc, x);
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

void Shader::SetVec2(const std::string &name, const glm::vec2 &vector) const
{
   int loc = GetUniformLocation(name);
   glUniform2f(loc, vector.x, vector.y);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &vector) const
{
   int loc = GetUniformLocation(name);
   glUniform3f(loc, vector.x, vector.y, vector.z);
}