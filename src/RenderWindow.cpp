#include "../include/RenderWindow.h"
#include "../include/glad/glad.h"
#include "../include/glm/gtx/projection.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
//#define NO_GL_DEBUGGING 1
#include "../include/GlDebug.h"
#include <cstdlib>
#include <string>

const static std::string VERTEX_STRING = 
{
 "    #version 460 core\n"
    "    layout (location = 0) in vec2 Pos;\n"
    "    layout (location = 1) in vec4 Color;\n"
    "    layout (location = 2) in vec2 UV;\n"
    "    layout (location = 3) in uint TexID;\n"
    "    layout (location = 4) in float Rotation;\n"
    "    layout (location = 5) in uint type;\n"
    "    out vec4  vColor;\n"
    "    out vec2  vUV;\n"
    "    out uint  vTexID;\n"
    "    out uint  vType;\n"
    "    uniform mat4 uProjection;\n"
    "    uniform mat4 uView;\n"
    "    void main()\n"
    "    {\n"
    "        gl_Position = uView * (uProjection * vec4(Pos, 1.0, 1.0));\n"
    "        vColor = Color;\n"
    "        vUV = UV;\n"
    "        vTexID = TexID;\n"
    "        vType = type;\n"
    "    }\n"
};

const static std::string FRAGMENT_STRING = 
{
 "#version 460 core\n"
    "in vec4 vColor;\n"
    "in vec2 vUV;\n"
    "in flat uint vTexID;\n"
    "in flat uint vType;\n"
    "out vec4 outColor;\n"
    "uniform sampler2D uTextures[32];\n"
    "void main()\n"
    "{\n"
    "   vec4 color = vec4(0.0);\n"
    "   switch(vType)\n"
    "   {\n"
    "       case 0:\n"
    "       color = texture(uTextures[vTexID], vUV);\n"
    "       if(color.r > 0.1)\n"
    "       {\n"
    "           outColor = vColor;\n"
    "           outColor.a = color.r;\n"
    "       }\n"
    "       else\n"
    "       {\n"
    "           discard;\n"
    "       }\n"
    "       break;\n"
    "       case 1:\n"
    "           outColor = vec4(vColor);\n"
    "       break;\n"
    "       case 2:\n"
    "           outColor = texture(uTextures[vTexID], vUV);\n"
    "           if(outColor.a < 0.1)\n"
    "           {\n"
    "               discard;\n"
    "           }\n"
    "       break;\n"
    "       case 3:\n"
    "           color = texture(uTextures[vTexID], vUV);\n"
    "           if(color.r > 0.3)\n"
    "           {\n"
    "               outColor = vColor;\n"
    "               outColor.a = color.r;\n"
    "           }\n"
    "           else\n"
    "           {\n"
    "               discard;\n"
    "           }\n"
    "       break;\n"
    "       default:\n"
    "           discard;\n"
    "       break;\n"
    "   }\n"
    "}\n"
};
static Texture* CircleTexture = nullptr;

static VertexArray     *m_vao = nullptr;
static VertexBuffer    *m_vertexBuffer = nullptr;
static IndexBuffer     *m_indexBuffer = nullptr;
static BufferLayout    m_layout;
static Shader          *m_shader = nullptr;

static glm::mat4        m_projection = glm::mat4(1.0f);
static glm::mat4        m_view       = glm::mat4(1.0f);

constexpr static ui32 MAX_VERTEX_COUNT  = 100000;
static i32 MAX_TEXTURE_COUNT = 16;

static std::vector<Vertex>  m_vertexData;
static std::vector<ui32>    m_indexData;
static std::vector<ui32>    m_textures;

// Generates an anti-aliased circle texture
inline void GenerateCircle(ui32 s, ui8* data)
{
    const float sHalf = s / 2.f;
    float d = 0.f;
    for(int i = 1; i < s-1; ++i)
    {
        for(int j = 1; j < s-1; ++j)
        {
            if(d = glm::distance(glm::vec2(j,i), glm::vec2(sHalf, sHalf)) < sHalf)
                data[i*s+j] = 255;
            else
                data[i*s+j] = 0 + (255 * (sHalf / d));
        }
    }
}

void PrintInformation()
{
    glad_glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_COUNT);
    const GLubyte* vendor = glad_glGetString(GL_VENDOR);
    const GLubyte* renderer = glad_glGetString(GL_RENDERER);
    tl::Logger::InfoMessage("Available GPU texture units: %d", MAX_TEXTURE_COUNT);
    tl::Logger::InfoMessage("GPU: %s %s", vendor, renderer);
}

void Renderer::Init()
{
    m_vertexData.reserve(MAX_VERTEX_COUNT);
    m_indexData.reserve(MAX_VERTEX_COUNT * 1.5);
    gladLoadGL();
    PrintInformation();

    m_vao           = new VertexArray();
    m_vertexBuffer  = new VertexBuffer(nullptr, sizeof(Vertex), MAX_VERTEX_COUNT);
    m_indexBuffer   = new IndexBuffer(nullptr, MAX_VERTEX_COUNT);
    m_shader        = new Shader();

    m_vao->Bind();
//    m_vertexBuffer->BufferData(0, sizeof(Vertex), MAX_VERTEX_COUNT);
//    m_indexBuffer->BufferData(0, MAX_VERTEX_COUNT*1.5);
    m_layout.Push(2, 4);
    m_layout.Push(4, 4);
    m_layout.Push(2, 4);
    m_layout.Push(1, 4);
    m_layout.Push(1, 4);
    m_layout.Push(1, 4);
    m_shader->FromString(VERTEX_STRING, FRAGMENT_STRING);
    m_shader->Bind();

    const int s = 4096;
    ui8* circleData = new ui8[s*s];

    GenerateCircle(s, circleData);

    CircleTexture = new Texture();
    CircleTexture->LoadFromMemory(s, s, 1, circleData);
    delete[] circleData;
    glad_glEnable(GL_BLEND);
    GL_CALL(glad_glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::Clear()
{
    glad_glClear(GL_COLOR_BUFFER_BIT);
	static int f[4];
	glad_glGetIntegerv(GL_VIEWPORT, f);
	m_projection = glm::ortho(f[0]*1.f, f[2]*1.f, f[3]*1.f, f[1]*1.f);
    GL_CALL(BeginBatch());
}

void Renderer::BeginBatch()
{
    m_vertexData.clear();
    m_indexData.clear();
    m_textures.clear();
}

void Renderer::Draw(Rectangle& r)
{
    ui32 currentElements = m_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4)
    {
        EndBatch();
        currentElements = 0;
    }
    if(r.m_tex.HasData() && m_textures.size() >= MAX_TEXTURE_COUNT - 2)
    {
        EndBatch();
        currentElements = 0;
    }
    
    const Color c = r.m_color * 0.003921568f;
    Vector2 origin;
    origin.x = (r.m_pos.x + r.m_pos.x + r.m_size.x) * 0.5f;
    origin.y = (r.m_pos.y + r.m_pos.y + r.m_size.y) * 0.5f;

    m_vertexData.push_back({Rotate(origin, r.m_pos, r.m_rotation),                          c, Vector2{0,0}, 0, r.m_rotation, RECTANGLE});
    m_vertexData.push_back({Rotate(origin, r.m_pos+Vector2{r.m_size.x, 0.0f}, r.m_rotation),c, Vector2{1,0}, 0, r.m_rotation, RECTANGLE});
    m_vertexData.push_back({Rotate(origin, r.m_pos+Vector2{0.0f, r.m_size.y},r.m_rotation), c, Vector2{0,1}, 0, r.m_rotation, RECTANGLE});
    m_vertexData.push_back({Rotate(origin, r.m_pos+r.m_size, r.m_rotation),                 c, Vector2{1,1}, 0, r.m_rotation, RECTANGLE});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
}

void Renderer::Draw(Circle& r)
{
    ui32 currentElements = m_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4)
    {
        EndBatch();
        currentElements = 0;
    }
    if(m_textures.size() > MAX_TEXTURE_COUNT - 1)
    {
        EndBatch();
        currentElements = 0;
    }

    ui32 tex = 0;
    bool already_in_m_textures = false;
    auto id = CircleTexture->GetID();
    ui32 index = 0;
    for(auto i : m_textures)
    {
        if(i == id)
        {
            already_in_m_textures = true;
            break;
        }
        ++index;
    }
    if(!already_in_m_textures)
    {
        tex = 1 + m_textures.size();
        CircleTexture->Bind(tex);
        m_textures.push_back(CircleTexture->GetID());
    }
    else
    {
        tex = index+1;
        CircleTexture->Bind(tex);
    }
        
    const Color c = r.m_color * 0.003921568f;

    m_vertexData.push_back({r.m_pos+Vector2{-r.m_size.x, -r.m_size.x},  c, Vector2{0,1}, tex, 0, CIRCLE});
    m_vertexData.push_back({r.m_pos+Vector2{r.m_size.x, -r.m_size.x},   c, Vector2{1,1}, tex, 0, CIRCLE});
    m_vertexData.push_back({r.m_pos+Vector2{-r.m_size.x, r.m_size.x},   c, Vector2{0,0}, tex, 0, CIRCLE});
    m_vertexData.push_back({r.m_pos+r.m_size,                           c, Vector2{1,0}, tex, 0, CIRCLE});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
}

void Renderer::Draw(Sprite& r)
{
    ui32 currentElements = m_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4)
    {
        EndBatch();
        currentElements = 0;
    }
    if(m_textures.size() >= MAX_TEXTURE_COUNT - 1)
    {
        EndBatch();
        currentElements = 0;
    }
    
    ui32 tex = 0;
    if(r.m_tex.GetID() != UINT_MAX)
    {
        bool already_in_m_textures = false;
        auto id = r.m_tex.GetID();
        ui32 index = 0;
        for(auto i : m_textures)
        {
            if(i == id)
            {
                already_in_m_textures = true;
                break;
            }
            ++index;
        }
        if(!already_in_m_textures)
        {
            tex = 1 + m_textures.size();
            r.m_tex.Bind(tex);
            m_textures.push_back(r.m_tex.GetID());
        }
        else
        {
            tex = index+1;
            r.m_tex.Bind(tex);
        }
    }
    else {
        return;
    }
    Vector2 origin;
    origin.x = (r.m_pos.x + r.m_pos.x + r.m_size.x) * 0.5f;
    origin.y = (r.m_pos.y + r.m_pos.y + r.m_size.y) * 0.5f;
    const Vector2 v = Vector2{1.f,1.f} / r.m_tex.GetSize();

    m_vertexData.push_back({Rotate(origin, r.m_pos, r.m_rotation),                          {0}, r.m_rect.pos * v, tex, r.m_rotation, TEXTURE});
    m_vertexData.push_back({Rotate(origin, r.m_pos+Vector2{r.m_size.x, 0.0f}, r.m_rotation),{0}, (r.m_rect.pos + Vector2{r.m_rect.size.x, 0}) * v, tex, r.m_rotation, TEXTURE});
    m_vertexData.push_back({Rotate(origin, r.m_pos+Vector2{0.0f, r.m_size.y},r.m_rotation), {0}, (r.m_rect.pos + Vector2{0, r.m_rect.size.y}) * v, tex, r.m_rotation, TEXTURE});
    m_vertexData.push_back({Rotate(origin, r.m_pos+r.m_size, r.m_rotation),                 {0}, (r.m_rect.pos + r.m_rect.size) * v, tex, r.m_rotation, TEXTURE});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
}

void Renderer::Draw(Text& r)
{
    ui32 currentElements = m_vertexData.size();
    if(currentElements + r.m_vertexData.size() >= MAX_VERTEX_COUNT)
    {
        EndBatch();
        currentElements = 0;
    }
    if(m_textures.size() >= MAX_TEXTURE_COUNT - 1)
    {
        EndBatch();
        currentElements = 0;
    }
    
    ui32 tex = 0;
    if(r.m_font.m_texture.GetID() != UINT_MAX)
    {
        bool already_in_m_textures = false;
        auto id = r.m_font.m_texture.GetID();
        ui32 index = 0;
        for(auto i : m_textures)
        {
            if(i == id)
            {
                already_in_m_textures = true;
                break;
            }
            ++index;
        }
        if(!already_in_m_textures)
        {
            tex = 1 + m_textures.size();
            r.m_font.m_texture.Bind(tex);
            m_textures.push_back(r.m_font.m_texture.GetID());
        }
        else
        {
            tex = index+1;
            r.m_font.m_texture.Bind(tex);
        }
    }
    else {
        return;
    }
    for(auto& v : r.m_vertexData)
    {
        v.tex = tex;
        m_vertexData.push_back(v);
    }
    for(auto i : r.m_indexData)
        m_indexData.push_back(currentElements + i);
}

void Renderer::EndBatch()
{
    if(m_vertexData.size() < 3)
        return;
    GL_CALL(m_shader->Bind());
    GL_CALL(m_shader->UniformMat4fv("uProjection", 1, 0, &m_projection[0][0]));
    GL_CALL(m_shader->UniformMat4fv("uView", 1, 0, &m_view[0][0]));
    for(i32 i = 1; i < 4; i++)
    {
        GL_CALL(m_shader->Uniform1i("uTextures[" + std::to_string(i) + "]", i));
    }
    if(m_vertexData.data())
    {
        m_vertexBuffer->SetData(m_vertexData.data(), sizeof(Vertex), m_vertexData.size());
        m_indexBuffer->SetData(m_indexData.data(), m_indexData.size());
    }
    else
    { // No data, so just return
        Renderer::BeginBatch();
        return;
    }

    m_vao->BufferData(*m_vertexBuffer, *m_indexBuffer, m_layout);
    m_vao->Bind();
    glad_glDrawElements(GL_TRIANGLES, m_indexData.size(), GL_UNSIGNED_INT, 0);

    Renderer::BeginBatch();
}
