#include "../include/RenderWindow.h"
#include "../include/Utilities/Bezier.h"
#include "../external-headers/glad/glad.h"
#include "../include/GlDebug.h"
#include "internal/Assert.h"
#include "internal/Circle_texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "../external-headers/stb/stb_image.h"


const static std::string VERTEX_STRING =
R"END(
#version 450 core
layout (location = 0) in mediump vec2 Pos;
layout (location = 1) in mediump vec4 Color;
layout (location = 2) in mediump vec2 UV;
layout (location = 3) in mediump uint TexID;
layout (location = 4) in mediump float Rotation;
layout (location = 5) in uint type;

out vec4  vColor;
out vec2  vUV;
out uint  vTexID;
out uint  vType;

uniform vec2 uViewSize;
uniform vec2 uCamPos;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uScale;

void main()
{
    vec4 r = uView * vec4(Pos-(uViewSize*0.5), 1, 1);
    gl_Position = uProj * uScale * vec4(r.xy + (uViewSize*0.5), 0, 1);
    vColor = Color;
    vUV = UV;
    vTexID = TexID;
    vType = type;
}
)END";

const static std::string FRAGMENT_STRING =
R"END(
#version 450 core
in vec4 vColor;
in vec2 vUV;
flat in uint vTexID;
flat in uint vType;
out vec4 outColor;
uniform sampler2D uTextures[32];
mat4 bt601 = mat4(
  1.16438,  0.00000,  1.59603, -0.87079,
  1.16438, -0.39176, -0.81297,  0.52959,
  1.16438,  2.01723,  0.00000, -1.08139,
  0, 0, 0, 1
);
void main()
{
   vec4 color = vec4(0.0);
   switch(vType)
   {
       case 0:
           color = texture(uTextures[vTexID], vUV);
           if(color.r > 0.02)
           {
               outColor = vColor;
               outColor.a = color.r * vColor.a;
           }
           else
           {
               discard;
           }
       break;
       case 1:
           outColor = vColor;
       break;
       case 2:
           outColor = texture(uTextures[vTexID], vUV);
           if(outColor.a < 0.1)
           {
               discard;
           }
       break;
       case 3:
           color = texture(uTextures[vTexID], vUV);
           if(color.r > 0.02)
           {
               outColor = vColor;
               outColor.a = color.r * vColor.a;
           }
           else
           {
               discard;
           }
       break;
       case 4:
           color = texture(uTextures[vTexID], vUV) * bt601;
       break;
       default:
           discard;
       break;
   }
}
)END";
using namespace tml;

static Texture*         CircleTexture = nullptr;
static VertexArray*     m_vao = nullptr;
static VertexBuffer*    m_vertexBuffer = nullptr;
static IndexBuffer*     m_indexBuffer = nullptr;
static BufferLayout     m_layout;
static Shader*          m_shader = nullptr;

static glm::mat4 m_view = glm::mat4(1.f);
static glm::mat4 m_proj = glm::mat4(1.f);
static glm::mat4 m_scale = glm::mat4(1.f);
static float m_zoom = 1;
static Vector2  m_viewSize   = {0,0};
static Vector2  m_camPos     = {0,0};

static Camera m_camera;

constexpr static ui32 MAX_VERTEX_COUNT  = 100000;
static i32 MAX_TEXTURE_COUNT = 16;

static std::vector<Vertex>  m_vertexData;
static std::vector<ui32>    m_indexData;
static std::vector<ui32>    m_textures;
int Renderer::batch_count = 0;

void PrintInformation()
{
    glad_glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_COUNT);
    const GLubyte* vendor = glad_glGetString(GL_VENDOR);
    const GLubyte* renderer = glad_glGetString(GL_RENDERER);
    const GLubyte* version = glad_glGetString(GL_VERSION);

    i32 max_tex_size = 0;
    glad_glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    tml::Logger::InfoMessage("GPU: %s %s", vendor, renderer);
    tml::Logger::InfoMessage("OpenGL version: %s", version);
    tml::Logger::InfoMessage("GPU max texture size: %d", max_tex_size);
    tml::Logger::InfoMessage("Available GPU texture units: %d", MAX_TEXTURE_COUNT);
}

void Renderer::Init()
{
    m_vertexData.reserve(MAX_VERTEX_COUNT);
    m_indexData.reserve(MAX_VERTEX_COUNT * 1.5);
    TML_ASSERT(gladLoadGL(), "Failed to initialize OpenGL.");
    PrintInformation();

    m_vao           = new VertexArray();
    m_vertexBuffer  = new VertexBuffer(nullptr, sizeof(Vertex), MAX_VERTEX_COUNT);
    m_indexBuffer   = new IndexBuffer(nullptr, MAX_VERTEX_COUNT * 1.5);
    m_shader        = new Shader();

    m_vao->Bind();
    m_layout.Push(2, 4);
    m_layout.Push(4, 4);
    m_layout.Push(2, 4);
    m_layout.Push(1, 4);
    m_layout.Push(1, 4);
    m_layout.Push(1, 4);
    m_shader->FromString(VERTEX_STRING, FRAGMENT_STRING);
    m_shader->Bind();
    int w = 0,h = 0,bpp = 0;
    ui8* circleData = stbi_load_from_memory(CIRCLE_TEXTURE_DATA.data(), CIRCLE_TEXTURE_DATA.size(), &w, &h, &bpp, 1);
    CircleTexture = new Texture();
    CircleTexture->LoadFromMemory(w, h, bpp, circleData);
    delete[] circleData;
    GL_CALL(glad_glEnable(GL_BLEND));
    GL_CALL(glad_glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Renderer::SetClearColor(const Color &color)
{
    GL_CALL(glad_glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
}

void Renderer::SetCamera(Camera &cam)
{
    m_camera = cam;
    m_view = glm::translate(m_view, glm::vec3(-cam.GetPosition().x, -cam.GetPosition().y, 0));
    m_view = glm::scale(m_view, glm::vec3(cam.GetZoom(), cam.GetZoom(), 0));
}

void Renderer::ResetCamera()
{
    EndBatch();
    m_view = glm::mat4(1.f);
    m_scale = glm::mat4(1.f);
    m_zoom = 1.f;
    m_camPos = {0,0};
}

void Renderer::Clear()
{
    batch_count = 0;
    GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT));
	static int f[4];
	GL_CALL(glad_glGetIntegerv(GL_VIEWPORT, f));
    m_viewSize = Vector2{static_cast<float>(f[2]), static_cast<float>(f[3])};

    // Put these in a separate function ResetView()
    ResetCamera();

    m_proj = glm::ortho(f[0]*1.f, f[2]*1.f, f[3]*1.f, f[1]*1.f);
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

    m_vertexData.push_back({Util::Rotate(origin, r.m_pos, r.m_rotation),                          c, Vector2{0,0}, 0, r.m_rotation, Vertex::RECTANGLE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+Vector2{r.m_size.x, 0.0f}, r.m_rotation),c, Vector2{1,0}, 0, r.m_rotation, Vertex::RECTANGLE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+Vector2{0.0f, r.m_size.y},r.m_rotation), c, Vector2{0,1}, 0, r.m_rotation, Vertex::RECTANGLE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+r.m_size, r.m_rotation),                 c, Vector2{1,1}, 0, r.m_rotation, Vertex::RECTANGLE});

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

    m_vertexData.push_back({r.m_pos+Vector2{-r.m_size.x, -r.m_size.x},  c, Vector2{0,1}, tex, 0, Vertex::CIRCLE});
    m_vertexData.push_back({r.m_pos+Vector2{r.m_size.x, -r.m_size.x},   c, Vector2{1,1}, tex, 0, Vertex::CIRCLE});
    m_vertexData.push_back({r.m_pos+Vector2{-r.m_size.x, r.m_size.x},   c, Vector2{0,0}, tex, 0, Vertex::CIRCLE});
    m_vertexData.push_back({r.m_pos+r.m_size,                                c, Vector2{1,0}, tex, 0, Vertex::CIRCLE});

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

    m_vertexData.push_back({Util::Rotate(origin, r.m_pos, r.m_rotation),                          {0}, r.m_rect.pos * v, tex, r.m_rotation, Vertex::TEXTURE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+Vector2{r.m_size.x, 0.0f}, r.m_rotation),{0}, (r.m_rect.pos + Vector2{r.m_rect.size.x, 0}) * v, tex, r.m_rotation, Vertex::TEXTURE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+Vector2{0.0f, r.m_size.y},r.m_rotation), {0}, (r.m_rect.pos + Vector2{0, r.m_rect.size.y}) * v, tex, r.m_rotation, Vertex::TEXTURE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+r.m_size, r.m_rotation),                 {0}, (r.m_rect.pos + r.m_rect.size) * v, tex, r.m_rotation, Vertex::TEXTURE});

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
    if((currentElements + r.m_vertexData.size()) >= MAX_VERTEX_COUNT)
    {
        EndBatch();
        currentElements = 0;
    }
    else if(m_textures.size() >= (MAX_TEXTURE_COUNT - 1))
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

void Renderer::Draw(Video& r)
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

    m_vertexData.push_back({Util::Rotate(origin, r.m_pos, r.m_rotation),                          {0}, {0,0}, tex, r.m_rotation, Vertex::TEXTURE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+Vector2{r.m_size.x, 0.0f}, r.m_rotation),{0}, {1,0}, tex, r.m_rotation, Vertex::TEXTURE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+Vector2{0.0f, r.m_size.y},r.m_rotation), {0}, {0,1}, tex, r.m_rotation, Vertex::TEXTURE});
    m_vertexData.push_back({Util::Rotate(origin, r.m_pos+r.m_size, r.m_rotation),                 {0}, {1,1}, tex, r.m_rotation, Vertex::TEXTURE});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
}
void Renderer::DrawLine(const Vector2 &a, const Vector2 &b, float thickness, Color color, bool rounded)
{
    ui32 currentElements = m_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4)
    {
        EndBatch();
        currentElements = 0;
    }
    // dx and dy for normals
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;
    const Color bColor = color; // Needed non normalized
    color = color * 0.003921568f;

    m_vertexData.push_back({Vector2(-dy, dx).Normalized() * (thickness * 0.5f) + a, color, {0,0}, 0, 0, Vertex::RECTANGLE});
    m_vertexData.push_back({Vector2(dy, -dx).Normalized() * (thickness * 0.5f) + a, color, {0,0}, 0, 0, Vertex::RECTANGLE});
    m_vertexData.push_back({Vector2(-dy, dx).Normalized() * (thickness * 0.5f) + b, color, {0,0}, 0, 0, Vertex::RECTANGLE});
    m_vertexData.push_back({Vector2(dy, -dx).Normalized() * (thickness * 0.5f) + b, color, {0,0}, 0, 0, Vertex::RECTANGLE});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
    if(rounded)
    {
        Renderer::DrawCircle(a, thickness/2, bColor);
        Renderer::DrawCircle(b, thickness/2, bColor);
    }
}

void Renderer::p_DrawRect(const Vector2& pos, const Vector2& dimensions, const Color& color, float rotation)
{
    ui32 currentElements = m_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4)
    {
        EndBatch();
        currentElements = 0;
    }

    const Color c = color * 0.003921568f;
    Vector2 origin;
    origin.x = (pos.x + pos.x + dimensions.x) * 0.5f;
    origin.y = (pos.y + pos.y + dimensions.y) * 0.5f;

    m_vertexData.push_back({Util::Rotate(origin, pos, rotation),                          c, Vector2{0,0}, 0, rotation, Vertex::RECTANGLE});
    m_vertexData.push_back({Util::Rotate(origin, pos+Vector2{dimensions.x, 0.0f},rotation),c, Vector2{1,0}, 0, rotation, Vertex::RECTANGLE});
    m_vertexData.push_back({Util::Rotate(origin, pos+Vector2{0.0f, dimensions.y},rotation), c, Vector2{0,1}, 0, rotation, Vertex::RECTANGLE});
    m_vertexData.push_back({Util::Rotate(origin, pos+dimensions, rotation),                 c, Vector2{1,1}, 0, rotation, Vertex::RECTANGLE});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
}

void Renderer::DrawRect(const Vector2& pos, const Vector2& dimensions, const Color& color, float roundness, float rotation)
{
    if(roundness < 3.f) // If roundness is too low, just draw a regular rectangle
        p_DrawRect(pos, dimensions, color, rotation);
    else
    {
        Vector2 origin =
                {origin.x = (pos.x + pos.x + dimensions.x) * 0.5f,
                 origin.y = (pos.y + pos.y + dimensions.y) * 0.5f};
        p_DrawRect(pos+Vector2{0, roundness}, dimensions - Vector2{0, roundness*2}, color, rotation);
        p_DrawRect(pos+Vector2{roundness, 0}, dimensions - Vector2{roundness*2, 0}, color, rotation);

        DrawCircle(Util::Rotate(origin, pos+Vector2{roundness, roundness}, rotation), roundness, color);
        DrawCircle(Util::Rotate(origin, pos+Vector2{dimensions.x - roundness, roundness}, rotation), roundness, color);
        DrawCircle(Util::Rotate(origin,pos+Vector2{roundness,dimensions.y - roundness}, rotation), roundness, color);
        DrawCircle(Util::Rotate(origin,pos+dimensions-Vector2{roundness, roundness}, rotation), roundness, color);
    }
}

void Renderer::DrawCircle(const Vector2& pos, float radius, const Color& color)
{
    ui32 currentElements = m_vertexData.size();
    if(currentElements >= (MAX_VERTEX_COUNT - 4))
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

    const Color ncolor = color * 0.003921568f;

    m_vertexData.push_back({pos+Vector2{-radius, -radius},  ncolor, Vector2{0,1}, tex, 0, Vertex::CIRCLE});
    m_vertexData.push_back({pos+Vector2{radius, -radius},   ncolor, Vector2{1,1}, tex, 0, Vertex::CIRCLE});
    m_vertexData.push_back({pos+Vector2{-radius, radius},   ncolor, Vector2{0,0}, tex, 0, Vertex::CIRCLE});
    m_vertexData.push_back({pos+Vector2{radius, radius},    ncolor, Vector2{1,0}, tex, 0, Vertex::CIRCLE});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
}

void Renderer::DrawBezier(const Vector2 &a, const Vector2 &cp1, const Vector2 &b, const Vector2 &cp2, float thickness,
                          const Color &color, bool rounded, float step)
{
    Vector2 begin = a;
    for(float i = 0; i < 1.f; i += step)
    {
        const Vector2 end = Cubic(a,cp1,cp2,b,i);
        DrawLine(begin, end, thickness, color, rounded);
        begin = end;
    }
}

void Renderer::DrawBezier(const Vector2 &a, const Vector2 &cp, const Vector2 &b, float thickness,
                          const Color &color, bool rounded, float step)
{
    Vector2 begin = a;
    for(float i = 0; i < 1.f; i += step)
    {
        const Vector2 end = Quadratic(a,cp,b,i);
        DrawLine(begin, end, thickness, color, rounded);
        begin = end;
    }
}

void Renderer::EndBatch()
{
    if(m_vertexData.size() < 3)
        return;
    GL_CALL(m_shader->Bind());
    GL_CALL(m_shader->SetVec2("uViewSize", m_viewSize));
    GL_CALL(m_shader->SetVec2("uCamPos", m_camPos));
    GL_CALL(m_shader->UniformMat4fv("uView", 1, 0, &m_view[0][0]));
    GL_CALL(m_shader->UniformMat4fv("uProj", 1, 0, &m_proj[0][0]));
    GL_CALL(m_shader->UniformMat4fv("uScale", 1, 0, &m_scale[0][0]));
    GL_CALL(m_shader->Uniform1f("uZoom", m_zoom));


    for(i32 i = 1; i < MAX_TEXTURE_COUNT; i++)
        GL_CALL(m_shader->Uniform1i("uTextures[" + std::to_string(i) + "]", i));

    m_vertexBuffer->SetData(m_vertexData.data(), sizeof(Vertex), m_vertexData.size());
    m_indexBuffer->SetData(m_indexData.data(), m_indexData.size());
    m_vao->BufferData(*m_vertexBuffer, *m_indexBuffer, m_layout);
    m_vao->Bind();
    GL_CALL(glad_glDrawElements(GL_TRIANGLES, m_indexData.size(), GL_UNSIGNED_INT, 0));

    Renderer::BeginBatch();
    ++batch_count;
}
