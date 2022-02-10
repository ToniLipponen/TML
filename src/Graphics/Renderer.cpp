#include <TML/Graphics/Renderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLAD_GL_IMPLEMENTATION
#define GLAD_GLES2_IMPLEMENTATION
#define GLAD_EGL_IMPLEMENTATION
#define GLAD_WGL_IMPLEMENTATION
#define GLAD_GLX_IMPLEMENTATION
#include <GLHeader.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GlDebug.h"
#include "_Assert.h"
#include "TML/Graphics/Core/Buffers.h"
#include "TML/Graphics/Core/Shader.h"
#include "Shaders.h"
#include <Circle.h> // Circle texture data

namespace tml
{
    Renderer::Renderer()
    {
        Init();
    }
    Renderer::~Renderer()
    {
        delete m_vao;
        delete m_vertexBuffer;
        delete m_indexBuffer;
        delete m_shader;
    }
    bool Renderer::Init() noexcept
    {
        m_vertexData.reserve(MAX_VERTEX_COUNT);
        m_indexData.reserve(MAX_VERTEX_COUNT * 1.5);

        int result = 0;
#ifdef TML_USE_GLES
    #ifdef PLATFORM_WINDOWS
        result = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    #else
        result = gladLoadGLES2((GLADloadfunc)glfwGetProcAddress);
    #endif
#else
        result = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
#endif

        TML_ASSERT(result, "Failed to initialize OpenGL.");
        if(result == 0)
            return false;

        GL_CALL(glad_glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_COUNT));

#ifdef TML_USE_GLES
        MAX_TEXTURE_COUNT = 8;
#endif

        m_vao           = new VertexArray();
        m_vertexBuffer  = new VertexBuffer(nullptr, sizeof(Vertex), MAX_VERTEX_COUNT);
        m_indexBuffer   = new IndexBuffer(nullptr, MAX_VERTEX_COUNT * 1.5);
        m_layout        = new BufferLayout();
        m_shader        = new Shader();

        m_vao->Bind();
        m_layout->Push(2, 4, BufferLayout::VERTEX_FLOAT);
        m_layout->Push(2, 4, BufferLayout::VERTEX_FLOAT);
        m_layout->Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);
        m_layout->Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);
        m_layout->Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);

        m_shader->FromString(VERTEX_STRING, FRAGMENT_STRING);
        m_shader->Bind();

        Image circleImage;
        circleImage.LoadFromData(CIRCLE.data(), CIRCLE.size());
        m_circleTexture.LoadFromImage(circleImage);

        GL_CALL(glEnable(GL_BLEND));
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_CALL(glBlendEquation(GL_FUNC_ADD));

#ifndef TML_USE_GLES
    #ifndef TML_NO_GL_DEBUGGING
//        glEnable(GL_DEBUG_OUTPUT);
//        glDebugMessageCallback(GLMessageCallback, nullptr);
    #endif
#endif
        return true;
    }

    void Renderer::SetClearColor(const Color &color) noexcept
    {
        GL_CALL(glad_glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
    }

    void Renderer::SetCamera(const Camera &cam) noexcept
    {
        ResetCamera();
        const auto pos = cam.GetPosition();
        const auto zoom = cam.GetZoom();
        auto* view = reinterpret_cast<glm::mat4*>(m_view);

        *view = glm::rotate(*view, cam.GetRotation(), glm::vec3(0.f, 0.f, 1.f));
        *view = glm::scale(*view, glm::vec3(zoom, zoom, 0));
        *view = glm::translate(*view, glm::vec3(-pos.x, -pos.y, 0));
    }

    void Renderer::ResetCamera() noexcept
    {
        EndBatch();
        auto* view = reinterpret_cast<glm::mat4*>(m_view);
        auto* scale = reinterpret_cast<glm::mat4*>(m_scale);

        *view  = glm::mat4(1.0f);
        *scale = glm::mat4(1.0f);
    }

    void Renderer::SetBounds(const Vector2i& pos, const Vector2i& size) noexcept
    {
        EndBatch();
        GL_CALL(glad_glScissor(pos.x, m_viewport.size.y - pos.y - size.y, size.x, size.y));
        GL_CALL(glad_glEnable(GL_SCISSOR_TEST));
    }

    void Renderer::SetViewport(const Vector2i &pos, const Vector2i &size) noexcept
    {
        m_viewport = {pos, size};
        GL_CALL(glad_glViewport(pos.x, pos.y, size.x, size.y));

        *reinterpret_cast<glm::mat4*>(m_proj) = glm::ortho(
                static_cast<float>(pos.x),
                static_cast<float>(pos.x+size.x),
                static_cast<float>(pos.y+size.y),
                static_cast<float>(pos.y)
        );
    }

    void Renderer::ResetBounds() noexcept
    {
        EndBatch();
        GL_CALL(glad_glDisable(GL_SCISSOR_TEST));
    }

    void Renderer::Clear() noexcept
    {
        GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT));
        GL_CALL(BeginBatch());

        ResetCamera();
        ResetBounds();
    }

    void Renderer::Draw(const Rectangle& r) noexcept
    {
        PushVertexData(r.m_vertexData, r.m_indexData);
    }

    void Renderer::Draw(Circle& r) noexcept
    {
        PushVertexData(r.m_vertexData, r.m_indexData, m_circleTexture);
    }

    void Renderer::Draw(Sprite& r) noexcept
    {
        PushVertexData(r.m_vertexData, r.m_indexData, r.m_tex);
    }

    void Renderer::Draw(Text& r) noexcept
    {
        PushVertexData(r.m_vertexData, r.m_indexData, r.m_font.m_texture);
    }

    void Renderer::Draw(const Video& r) noexcept
    {
        if(m_textures.size() >= MAX_TEXTURE_COUNT - 3)
            EndBatch();
        PushQuad(r.m_pos, r.m_size, r.m_color, r.m_y, Vertex::VIDEO);
        PushTexture(r.m_cb);
        PushTexture(r.m_cr);
    }

    void Renderer::DrawLine(const Vector2f &a, const Vector2f &b, float thickness, Color color, bool rounded) noexcept
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

        m_vertexData.push_back({((Vector2f(-dy, dx).Normalized() * thickness * 0.5) + a), {0, 0}, color.Hex(), 0, Vertex::COLOR});
        m_vertexData.push_back({((Vector2f(dy, -dx).Normalized() * thickness * 0.5) + a), {0, 0}, color.Hex(), 0, Vertex::COLOR});
        m_vertexData.push_back({((Vector2f(-dy, dx).Normalized() * thickness * 0.5) + b), {0, 0}, color.Hex(), 0, Vertex::COLOR});
        m_vertexData.push_back({((Vector2f(dy, -dx).Normalized() * thickness * 0.5) + b), {0, 0}, color.Hex(), 0, Vertex::COLOR});

        m_indexData.push_back(currentElements + 0);
        m_indexData.push_back(currentElements + 1);
        m_indexData.push_back(currentElements + 2);

        m_indexData.push_back(currentElements + 1);
        m_indexData.push_back(currentElements + 3);
        m_indexData.push_back(currentElements + 2);

        if(rounded) // Doesn't work well with translucent colors. Might do something to fix this in some point. TODO
        {
            Renderer::DrawCircle(a, thickness * 0.5f, color);
            Renderer::DrawCircle(b, thickness * 0.5f, color);
        }
    }

    void Renderer::DrawRect(const Vector2f& pos, const Vector2f& dimensions, const Color& color, float roundness, float rotation) noexcept
    {
        if(roundness < 3.f) // If roundness is too low, just draw a single quad
            PushQuad(pos, dimensions, color, m_circleTexture, Vertex::COLOR, rotation);
        else
        {
            // Doesn't work well with translucent colors. Might do something to fix this in some point. TODO
            const Vector2f origin = {(pos.x + pos.x + dimensions.x) * 0.5f,
                                     (pos.y + pos.y + dimensions.y) * 0.5f};
            PushQuad(pos+Vector2f{0.f, roundness}, dimensions - Vector2f{0.f, roundness*2}, color, m_circleTexture, Vertex::COLOR, rotation);
            PushQuad(pos+Vector2f{roundness, 0.f}, dimensions - Vector2f{roundness*2, 0.f}, color, m_circleTexture, Vertex::COLOR, rotation);

            DrawCircle(Math::Rotate(origin, pos+Vector2f{roundness, roundness}, rotation), roundness, color);
            DrawCircle(Math::Rotate(origin, pos+Vector2f{dimensions.x - roundness, roundness}, rotation), roundness, color);
            DrawCircle(Math::Rotate(origin, pos+Vector2f{roundness,dimensions.y - roundness}, rotation), roundness, color);
            DrawCircle(Math::Rotate(origin, pos+dimensions-Vector2f{roundness, roundness}, rotation), roundness, color);
        }
    }

    void Renderer::DrawCircle(const Vector2f& pos, float radius, const Color& color) noexcept
    {
        PushQuad(pos - Vector2f{radius}, {radius * 2}, color, m_circleTexture, Vertex::TEXT);
    }

    void Renderer::DrawBezier(const Vector2f &a, const Vector2f &cp1, const Vector2f &cp2, const Vector2f &b, float thickness,
                              const Color &color, bool rounded, float step) noexcept
    {
        Vector2f begin = a;
        for(float i = 0; i < 1.f; i += step)
        {
            const Vector2f end = Math::Cubic(a,cp1,cp2,b,i);
            DrawLine(begin, end, thickness, color, rounded);
            begin = end;
        }
    }

    void Renderer::DrawBezier(const Vector2f &a, const Vector2f &cp, const Vector2f &b, float thickness,
                              const Color &color, bool rounded, float step) noexcept
    {
        Vector2f begin = a;
        for(float i = 0; i < 1.f; i += step)
        {
            const Vector2f end = Math::Quadratic(a,cp,b,i);
            DrawLine(begin, end, thickness, color, rounded);
            begin = end;
        }
    }

    void Renderer::DrawGrid(const Vector2f &top_left, const Vector2f &size, ui32 rows, ui32 columns, const Color &color,
                            float thickness, bool rounded) noexcept
    {
        for(int i = 0; i <= rows; ++i)
        {
            DrawLine(top_left + Vector2f{0.f,    (size.y / rows) * i},
                     top_left + Vector2f{size.x, (size.y / rows) * i}, thickness, color, ((i == 0) || (i == rows)));
        }
        for(int i = 0; i <= columns; ++i)
            DrawLine(top_left + Vector2f{(size.x / columns) * i,0.f},
                     top_left + Vector2f{(size.x / columns) * i, size.y}, thickness, color, false);
    }

    void Renderer::DrawTexture(const Texture &tex, const Vector2f &pos, const Vector2f &size) noexcept
    {
        PushQuad(pos, size, Color::Transparent, tex, Vertex::TEXTURE);
    }

    void Renderer::DrawTextureRect(const Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& tl, const Vector2f& br) noexcept
    {
        PushQuad(pos, size, Color::Transparent, tex, Vertex::TEXTURE, rotation, tl, br);
    }

    void Renderer::PushQuad(const Vector2f &pos,
                            const Vector2f &size,
                            const Color &col,
                            const Texture& texture,
                            Vertex::DrawableType type,
                            float rotation,
                            const Vector2f& tl,
                            const Vector2f& br) noexcept
    {
        ui32 currentElements = m_vertexData.size();
        if(currentElements >= MAX_VERTEX_COUNT - 4)
            EndBatch();

        const ui32 tex = PushTexture(texture);
        const ui32 hex = col.Hex();

        currentElements = m_vertexData.size(); // PushTexture() might have ended the last batch, so we need to get the m_vertexData.size() again

        if(rotation != 0)
        {
            const Vector2f origin = (pos + pos + size) * 0.5f;
            m_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos, rotation), tl, hex, tex, type});
            m_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + Vector2f{size.x, 0.f}, rotation), {br.x, tl.y}, hex, tex, type});
            m_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + Vector2f{0.f, size.y}, rotation), {tl.x, br.y}, hex, tex, type});
            m_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + size, rotation), br, hex, tex, type});
        }
        else
        {
            m_vertexData.emplace_back(Vertex{pos, tl, hex, tex, type});
            m_vertexData.emplace_back(Vertex{pos + Vector2f{size.x, 0.f}, {br.x, tl.y}, hex, tex, type});
            m_vertexData.emplace_back(Vertex{pos + Vector2f{0.f, size.y}, {tl.x, br.y}, hex, tex, type});
            m_vertexData.emplace_back(Vertex{pos + size, br, hex, tex, type});
        }

        m_indexData.emplace_back(currentElements + 0);
        m_indexData.emplace_back(currentElements + 1);
        m_indexData.emplace_back(currentElements + 2);

        m_indexData.emplace_back(currentElements + 1);
        m_indexData.emplace_back(currentElements + 3);
        m_indexData.emplace_back(currentElements + 2);
    }

    // Finds a parking spot for the texture.
    ui32 Renderer::PushTexture(const Texture &texture) noexcept
    {
        if(m_textures.size() >= MAX_TEXTURE_COUNT)
            EndBatch();

        bool alreadyInMTextures = false;
        const auto id = texture.GetID();
        ui32 index = 0;

        for(const auto i : m_textures)
        {
            if(i == id)
            {
                alreadyInMTextures = true;
                break;
            }
            ++index;
        }

        if(!alreadyInMTextures)
        {
            texture.Bind(index);
            m_textures.push_back(id);
        }

        return index;
    }

#ifdef PLATFORM_WINDOWS
    #undef DrawText
#endif
    void Renderer::DrawText(const String& text, const Vector2f& pos, float size, const Color& color) noexcept
    {
        m_text.SetString(text);
        m_text.SetSize(size);
        m_text.SetColor(color);
        m_text.SetPosition(pos);
        Draw(m_text);
    }
    
    void Renderer::PushVertexData(const std::vector<Vertex>& vertices, const std::vector<ui32>& indices) noexcept
    {
        if(MAX_VERTEX_COUNT <= m_vertexData.size() + vertices.size())
            EndBatch();
        const auto size = m_vertexData.size();
        std::copy(vertices.begin(), vertices.end(), std::back_inserter(m_vertexData));
        for(const auto i : indices)
            m_indexData.push_back(size + i);
    }

    void Renderer::PushVertexData(std::vector<Vertex>& vertices, const std::vector<ui32>& indices, const Texture& tex) noexcept
    {
        const ui32 slot = PushTexture(tex);
        for(Vertex& i : vertices)
            i.tex = slot;
        PushVertexData(vertices, indices);
    }

    void Renderer::EndBatch(bool flip) noexcept
    {
        m_shader->Bind();

        for(i32 i = 0; i < m_textures.size(); i++)
            m_shader->Uniform1i("uTexture" + std::to_string(i), i);

        if(flip)
            m_view[5] = -m_view[5];

        m_shader->Uniform2f("uViewSize", m_viewport.size.x, m_viewport.size.y);
        m_shader->UniformMat4fv("uView",  1, false, m_view);
        m_shader->UniformMat4fv("uProj",  1, false, m_proj);
        m_shader->UniformMat4fv("uScale", 1, false, m_scale);

        m_vertexBuffer->PushData(m_vertexData.data(), sizeof(Vertex), m_vertexData.size());
        m_indexBuffer->PushData(m_indexData.data(), m_indexData.size());
        m_vao->BufferData(*m_vertexBuffer, *m_indexBuffer, *m_layout);

        m_vao->Bind();
        m_vertexBuffer->Bind();
        m_indexBuffer->Bind();

        GL_CALL(glad_glDrawElements(GL_TRIANGLES, m_indexBuffer->Elements(), GL_UNSIGNED_INT, nullptr));
        Renderer::BeginBatch();

        if(flip)
            m_view[5] = -m_view[5];
    }

    void Renderer::BeginBatch() noexcept
    {
        m_vertexData.clear();
        m_indexData.clear();
        for(auto& i : m_textures)
            i = -1;
        m_textures.clear();
        m_vertexBuffer->Flush();
        m_indexBuffer->Flush();
    }
}