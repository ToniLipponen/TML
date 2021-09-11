#include <TML/RenderWindow.h>
#include <TML/Utilities/Bezier.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <string>

#include "internal/GlDebug.h"
#include "internal/Assert.h"
#include "internal/Circle_texture.h"
#include "internal/Buffers.h"
#include "internal/Shader.h"


const static std::string VERTEX_STRING =
R"END(
#version 450 core
layout (location = 0) in vec2 Pos;
layout (location = 1) in uint Color;
layout (location = 2) in vec2 UV;
layout (location = 3) in uint Tex;
layout (location = 4) in uint Type;

out vec4  vColor;
out vec2  vUV;
out uint  vTexID;
out uint  vType;

uniform vec2 uViewSize;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uScale;

vec4 IntToVec4(uint c)
{
    return vec4(
        (c & 0xff000000) >> 24,
        (c & 0x00ff0000) >> 16,
        (c & 0x0000ff00) >> 8,
        (c & 0x000000ff)) * 0.003921568;
}

void main()
{
    vec4 r = uView * vec4(Pos-(uViewSize*0.5), 1, 1);
    gl_Position = uProj * (uScale * vec4(r.xy + (uViewSize*0.5), 0, 1));
    vColor = IntToVec4(Color);
    vUV = UV;
    vTexID = Tex;
    vType = Type;
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
extern std::vector<ui8> DEFAULT_FONT_DATA;

static Texture*         s_circleTexture = nullptr;
static VertexArray*     s_vao = nullptr;
static VertexBuffer*    s_vertexBuffer = nullptr;
static IndexBuffer*     s_indexBuffer = nullptr;
static BufferLayout     s_layout;
static Shader*          s_shader = nullptr;
static Text*            s_text = nullptr;

static glm::mat4 s_view = glm::mat4(1.f);
static glm::mat4 s_proj = glm::mat4(1.f);
static glm::mat4 s_scale = glm::mat4(1.f);
static Vector2  s_viewSize   = {0, 0};

static Camera s_camera;

constexpr static ui32 MAX_VERTEX_COUNT  = 100000;
static i32 MAX_TEXTURE_COUNT = 16;

static std::vector<Vertex>  s_vertexData;
static std::vector<ui32>    s_indexData;
static std::vector<ui32>    s_textures;
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
    s_vertexData.reserve(MAX_VERTEX_COUNT);
    s_indexData.reserve(MAX_VERTEX_COUNT * 1.5);
    TML_ASSERT(gladLoadGL(), "Failed to initialize OpenGL.");
    PrintInformation();

    s_vao           = new VertexArray();
    s_vertexBuffer  = new VertexBuffer(nullptr, sizeof(Vertex), MAX_VERTEX_COUNT);
    s_indexBuffer   = new IndexBuffer(nullptr, MAX_VERTEX_COUNT * 1.5);
    s_shader        = new Shader();
    s_text          = new Text();

    s_vao->Bind();
    s_layout.Push(2, 4);
    s_layout.Push(1, 4);
    s_layout.Push(2, 4);
    s_layout.Push(1, 4);
    s_layout.Push(1, 4);
    s_shader->FromString(VERTEX_STRING, FRAGMENT_STRING);
    s_shader->Bind();
    int w = 0,h = 0,bpp = 0;
    ui8* circleData = stbi_load_from_memory(CIRCLE_TEXTURE_DATA.data(), static_cast<int>(CIRCLE_TEXTURE_DATA.size()), &w, &h, &bpp, 1);
    s_circleTexture = new Texture();
    s_circleTexture->LoadFromMemory(w, h, bpp, circleData);
    delete[] circleData;
    GL_CALL(glad_glEnable(GL_BLEND));
    GL_CALL(glad_glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glad_glDisable(GL_DEPTH_TEST));

}

void Renderer::SetClearColor(const Color &color)
{
    GL_CALL(glad_glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
}

void Renderer::SetCamera(Camera &cam)
{
    ResetCamera();
    s_camera = cam;
    s_camera.Move((s_viewSize / 2) * -1);
    s_view = glm::rotate(s_view, cam.GetRotation(), glm::vec3(0.f, 0.f, 1.f));
    s_view = glm::scale(s_view, glm::vec3(cam.GetZoom(), cam.GetZoom(), 0));
    s_view = glm::translate(s_view, glm::vec3(-s_camera.GetPosition().x, -s_camera.GetPosition().y, 0));
}

void Renderer::ResetCamera()
{
    EndBatch();
    s_view = glm::mat4(1.f);
    s_scale = glm::mat4(1.f);
}

void Renderer::Clear()
{
    batch_count = 0;
    GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT));
	static int f[4];
	GL_CALL(glad_glGetIntegerv(GL_VIEWPORT, f));
    s_viewSize = Vector2{static_cast<float>(f[2]), static_cast<float>(f[3])};
    ResetCamera();
    s_proj = glm::ortho(
            static_cast<float>(f[0]),
            static_cast<float>(f[2]),
            static_cast<float>(f[3]),
            static_cast<float>(f[1])
            );
    GL_CALL(BeginBatch());
}

void Renderer::BeginBatch()
{
    s_vertexData.clear();
    s_indexData.clear();
    s_textures.clear();
    s_vertexBuffer->Flush();
    s_indexBuffer->Flush();
}

void Renderer::Draw(Rectangle& r)
{
    PushQuad(r.m_pos, r.m_size, r.m_color, r.m_tex, r.m_rotation, Vertex::RECTANGLE);
}

void Renderer::Draw(Circle& r)
{
    PushQuad(r.m_pos - (r.m_size / 2), r.m_size, r.m_color, *s_circleTexture, Vertex::CIRCLE);
}

void Renderer::Draw(Sprite& r)
{
    PushQuad(r.m_pos, r.m_size, r.m_color, r.m_tex, r.m_rotation, Vertex::TEXTURE);
}

void Renderer::Draw(Text& r)
{
    ui32 currentElements = s_vertexData.size();
    if((currentElements + r.m_vertexData.size()) >= MAX_VERTEX_COUNT)
    {
        EndBatch();
        currentElements = 0;
    }
    else if(s_textures.size() >= (MAX_TEXTURE_COUNT - 1))
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
        for(auto i : s_textures)
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
            tex = 1 + s_textures.size();
            r.m_font.m_texture.Bind(tex);
            s_textures.push_back(r.m_font.m_texture.GetID());
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
        s_vertexData.push_back(v);
    }
    for(auto i : r.m_indexData)
        s_indexData.push_back(currentElements + i);
}

void Renderer::Draw(Video& r)
{
    PushQuad(r.m_pos, r.m_size, r.m_color, r.m_tex,Vertex::TEXTURE);
}

void Renderer::DrawLine(const Vector2 &a, const Vector2 &b, float thickness, Color color, bool rounded)
{
    ui32 currentElements = s_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4)
    {
        EndBatch();
        currentElements = 0;
    }
    // dx and dy for normals
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;

    s_vertexData.push_back({Vector2(-dy, dx).Normalized() * (thickness * 0.5f) + a, color.Hex(), {0, 0}, 0, Vertex::RECTANGLE});
    s_vertexData.push_back({Vector2(dy, -dx).Normalized() * (thickness * 0.5f) + a, color.Hex(), {0, 0}, 0, Vertex::RECTANGLE});
    s_vertexData.push_back({Vector2(-dy, dx).Normalized() * (thickness * 0.5f) + b, color.Hex(), {0, 0}, 0, Vertex::RECTANGLE});
    s_vertexData.push_back({Vector2(dy, -dx).Normalized() * (thickness * 0.5f) + b, color.Hex(), {0, 0}, 0, Vertex::RECTANGLE});

    s_indexData.push_back(currentElements + 0);
    s_indexData.push_back(currentElements + 1);
    s_indexData.push_back(currentElements + 2);

    s_indexData.push_back(currentElements + 1);
    s_indexData.push_back(currentElements + 3);
    s_indexData.push_back(currentElements + 2);
    if(rounded)
    {
        Renderer::DrawCircle(a, thickness/2, color);
        Renderer::DrawCircle(b, thickness/2, color);
    }
}

void Renderer::p_DrawRect(const Vector2& pos, const Vector2& dimensions, const Color& color, float rotation)
{
    ui32 currentElements = s_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4)
    {
        EndBatch();
        currentElements = 0;
    }

    const Color c = color;
    Vector2 origin = {(pos.x + pos.x + dimensions.x) * 0.5f,
                      (pos.y + pos.y + dimensions.y) * 0.5f};
    // Clean this up
    s_vertexData.push_back({Util::Rotate(origin, pos, rotation), c.Hex(), Vector2{0, 0}, 0, Vertex::RECTANGLE});
    s_vertexData.push_back({Util::Rotate(origin, pos + Vector2{dimensions.x, 0.0f}, rotation), c.Hex(), Vector2{1, 0}, 0, Vertex::RECTANGLE});
    s_vertexData.push_back({Util::Rotate(origin, pos + Vector2{0.0f, dimensions.y}, rotation), c.Hex(), Vector2{0, 1}, 0, Vertex::RECTANGLE});
    s_vertexData.push_back({Util::Rotate(origin, pos + dimensions, rotation), c.Hex(), Vector2{1, 1}, 0, Vertex::RECTANGLE});

    s_indexData.push_back(currentElements + 0);
    s_indexData.push_back(currentElements + 1);
    s_indexData.push_back(currentElements + 2);

    s_indexData.push_back(currentElements + 1);
    s_indexData.push_back(currentElements + 3);
    s_indexData.push_back(currentElements + 2);
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
        p_DrawRect(pos+Vector2{0.f, roundness}, dimensions - Vector2{0.f, roundness*2}, color, rotation);
        p_DrawRect(pos+Vector2{roundness, 0.f}, dimensions - Vector2{roundness*2, 0.f}, color, rotation);

        DrawCircle(Util::Rotate(origin, pos+Vector2{roundness, roundness}, rotation), roundness, color);
        DrawCircle(Util::Rotate(origin, pos+Vector2{dimensions.x - roundness, roundness}, rotation), roundness, color);
        DrawCircle(Util::Rotate(origin,pos+Vector2{roundness,dimensions.y - roundness}, rotation), roundness, color);
        DrawCircle(Util::Rotate(origin,pos+dimensions-Vector2{roundness, roundness}, rotation), roundness, color);
    }
}

void Renderer::DrawCircle(const Vector2& pos, float radius, const Color& color)
{
    PushQuad(pos - Vector2{radius,radius}, {radius*2}, color, *s_circleTexture, Vertex::CIRCLE);
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

void Renderer::DrawGrid(const Vector2 &top_left, const Vector2 &size, ui32 rows, ui32 columns, const Color &color,
                        float thickness, bool rounded)
{
    for(int i = 0; i <= rows; ++i)
    {
        DrawLine(top_left + Vector2{0.f,(size.y / rows) * i},
                 top_left + Vector2{size.x, (size.y / rows) * i}, thickness, color, ((i == 0) || (i == rows)));
    }
    for(int i = 0; i <= columns; ++i)
        DrawLine(top_left + Vector2{(size.x / rows) * i,0.f},
                 top_left + Vector2{(size.x / rows) * i, size.y}, thickness, color, false);
}

void Renderer::DrawTexture(Texture &tex, const Vector2 &pos, const Vector2 &size)
{
    PushQuad(pos, size, TRANSPARENT, tex, Vertex::TEXTURE);
}

void
Renderer::PushQuad(const Vector2 &pos, const Vector2 &size, const Color &col, Texture& texture, Vertex::Drawable_Type type)
{
    ui32 currentElements = s_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4 || s_textures.size() > MAX_TEXTURE_COUNT - 1)
    {
        EndBatch();
        currentElements = 0;
    }

    ui32 tex = 0;
    if(type != Vertex::RECTANGLE)
    {
        bool already_in_m_textures = false;
        auto id = texture.GetID();
        ui32 index= 0;
        for(auto i : s_textures)
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
            tex = 1 + s_textures.size();
            texture.Bind(tex);
            s_textures.push_back(tex);
        }
        else
        {
            tex = index+1;
            texture.Bind(tex);
        }
    }

    s_vertexData.push_back({pos, col.Hex(), {0.f, 0.f}, tex, type});
    s_vertexData.push_back({pos + Vector2{size.x, 0.f}, col.Hex(), {1.f, 0.f}, tex, type});
    s_vertexData.push_back({pos + Vector2{0.f, size.y}, col.Hex(), {0.f, 1.f}, tex, type});
    s_vertexData.push_back({pos + size, col.Hex(), {1.f, 1.f}, tex, type});

    s_indexData.push_back(currentElements + 0);
    s_indexData.push_back(currentElements + 1);
    s_indexData.push_back(currentElements + 2);

    s_indexData.push_back(currentElements + 1);
    s_indexData.push_back(currentElements + 3);
    s_indexData.push_back(currentElements + 2);
}

void
Renderer::PushQuad(const Vector2 &pos, const Vector2 &size, const Color &col, Texture& texture, float rotation, Vertex::Drawable_Type type)
{
    ui32 currentElements = s_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4 || s_textures.size() > MAX_TEXTURE_COUNT - 1)
    {
        EndBatch();
        currentElements = 0;
    }

    ui32 tex = 0;
    if(type != Vertex::RECTANGLE)
    {
        bool already_in_m_textures = false;
        auto id = texture.GetID();
        ui32 index= 0;
        for(auto i : s_textures)
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
            tex = 1 + s_textures.size();
            texture.Bind(tex);
            s_textures.push_back(tex);
        }
        else
        {
            tex = index+1;
            texture.Bind(tex);
        }
    }
//    const Vector2 origin = (pos+size) * 0.5f;
    const Vector2 origin = (pos + pos + size) * 0.5f;
    s_vertexData.push_back({Util::Rotate(origin, pos, rotation), col.Hex(), {0.f, 0.f}, tex, type});
    s_vertexData.push_back({Util::Rotate(origin, pos + Vector2{size.x, 0.f}, rotation), col.Hex(), {1.f, 0.f}, tex, type});
    s_vertexData.push_back({Util::Rotate(origin, pos + Vector2{0.f, size.y}, rotation), col.Hex(), {0.f, 1.f}, tex, type});
    s_vertexData.push_back({Util::Rotate(origin, pos + size, rotation), col.Hex(), {1.f, 1.f}, tex, type});

    s_indexData.push_back(currentElements + 0);
    s_indexData.push_back(currentElements + 1);
    s_indexData.push_back(currentElements + 2);

    s_indexData.push_back(currentElements + 1);
    s_indexData.push_back(currentElements + 3);
    s_indexData.push_back(currentElements + 2);
}

// TODO:
// Find out why this causes textures to glitch.
void Renderer::DrawText(const std::string &text, const Vector2 &pos, float size, const Color &color)
{
    s_text->SetString(text);
    s_text->SetSize(size);
    s_text->SetColor(color);
    s_text->SetPosition(pos);
    Draw(*s_text);
}

void Renderer::EndBatch()
{
    GL_CALL(s_shader->Bind());
    GL_CALL(s_shader->SetVec2("uViewSize", s_viewSize));
    GL_CALL(s_shader->UniformMat4fv("uView", 1, 0, &s_view[0][0]));
    GL_CALL(s_shader->UniformMat4fv("uProj", 1, 0, &s_proj[0][0]));
    GL_CALL(s_shader->UniformMat4fv("uScale", 1, 0, &s_scale[0][0]));
    GL_CALL(s_shader->Uniform1f("uZoom", s_camera.GetZoom()));

    for(i32 i = 1; i < MAX_TEXTURE_COUNT; i++)
        GL_CALL(s_shader->Uniform1i("uTextures[" + std::to_string(i) + "]", i));

    s_vertexBuffer->PushData(s_vertexData.data(), sizeof(Vertex), s_vertexData.size());
    s_indexBuffer->PushData(s_indexData.data(), s_indexData.size());
    s_vao->BufferData(*s_vertexBuffer, *s_indexBuffer, s_layout);
    s_vao->Bind();

    GL_CALL(glad_glDrawElements(GL_TRIANGLES, s_indexBuffer->Elements(), GL_UNSIGNED_INT, nullptr));

    Renderer::BeginBatch();
    ++batch_count;
}