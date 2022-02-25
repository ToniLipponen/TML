#include <TML/Graphics/Renderer.h>
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>

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

#include "Shaders.h"

namespace tml
{
    static constexpr void MakeCircle(Image& image, ui32 resolution) noexcept
    {
        auto* buffer = image.GetData();
        const auto radius = resolution / 2.0;
        const auto center = Vector2f(radius);

        for(auto i = 0; i < resolution; ++i)
            for(auto j = 0; j < resolution; ++j)
            {
                const double dist = Math::Distance(Vector2f(j, i), center);
                const double d = dist / radius;
                buffer[i * resolution + j] = static_cast<unsigned char>(Math::SmoothStep<double>(0.0, 0.002, 1.0 - d) * 255.0);
            }
    }

    Renderer::Renderer()
    {
        m_vertexData.reserve(MAX_VERTEX_COUNT);
        m_indexData.reserve(MAX_VERTEX_COUNT * 1.5);

#ifdef TML_USE_GLES
    #ifdef PLATFORM_WINDOWS
        TML_ASSERT(gladLoadGL((GLADloadfunc)glfwGetProcAddress), "Failed to initialize renderer");
    #else
        TML_ASSERT(gladLoadGLES2((GLADloadfunc)glfwGetProcAddress), "Failed to initialize renderer");
    #endif
#else
        TML_ASSERT(gladLoadGL((GLADloadfunc)glfwGetProcAddress), "Failed to initialize renderer");
#endif
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

        m_shader->LoadFromString(VERTEX_STRING, FRAGMENT_STRING);
        m_shader->Bind();

        Image circleImage;
        circleImage.LoadFromMemory(4096, 4096, 1, nullptr);
        MakeCircle(circleImage, 4096);
        m_circleTexture.LoadFromImage(circleImage);

        GL_CALL(glEnable(GL_BLEND));
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_CALL(glBlendEquation(GL_FUNC_ADD));

#ifndef TML_USE_GLES
    #ifndef TML_NO_GL_DEBUGGING
//            glEnable(GL_DEBUG_OUTPUT);
//            glDebugMessageCallback(GLMessageCallback, nullptr);
    #endif
#endif
    }
    Renderer::~Renderer()
    {
        delete m_vao;
        delete m_vertexBuffer;
        delete m_indexBuffer;
        delete m_layout;
        delete m_shader;
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

    void Renderer::Draw(Drawable& d) noexcept
    {
        d.OnDraw(this, &m_circleTexture);
    }

    void Renderer::DrawLine(const Vector2f &a, const Vector2f &b, ui32 thickness, Color color, bool rounded) noexcept
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
        const auto dirA = (Vector2f(-dy, dx).Normalized() * thickness * 0.5);
        const auto dirB = (Vector2f(dy, -dx).Normalized() * thickness * 0.5);

        m_vertexData.emplace_back(Vertex{(dirA + a), {0, 0}, color.Hex(), Vertex::COLOR});
        m_vertexData.emplace_back(Vertex{(dirB + a), {0, 0}, color.Hex(), Vertex::COLOR});
        m_vertexData.emplace_back(Vertex{(dirA + b), {0, 0}, color.Hex(), Vertex::COLOR});
        m_vertexData.emplace_back(Vertex{(dirB + b), {0, 0}, color.Hex(), Vertex::COLOR});

        m_indexData.push_back(currentElements + 0);
        m_indexData.push_back(currentElements + 1);
        m_indexData.push_back(currentElements + 2);

        m_indexData.push_back(currentElements + 1);
        m_indexData.push_back(currentElements + 3);
        m_indexData.push_back(currentElements + 2);

        if(rounded) /// Doesn't work well with translucent colors. Might do something to fix this in some point. TODO
        {
            Renderer::DrawCircle(a, thickness * 0.5f, color);
            Renderer::DrawCircle(b, thickness * 0.5f, color);
        }
    }

    void Renderer::DrawRect(const Vector2f& pos, const Vector2f& dimensions, const Color& color, float roundness, float rotation) noexcept
    {
        if(roundness < 1.f) // If roundness is too low, just draw a single quad
            PushQuad(pos, dimensions, color, m_circleTexture, Vertex::COLOR, rotation);
        else
        {
            roundness = Math::Clamp<float>(roundness, 0, Math::Min(dimensions.y, dimensions.x) / 2);

            /// Todo: Take origin as an argument.
            const Vector2f origin = (pos + pos + dimensions) * 0.5f;

            auto w = Vector2f{dimensions.x, 0.f};
            auto h = Vector2f{0.f, dimensions.y};
            auto rx = Vector2f{roundness, 0.f};
            auto ry = Vector2f{0.f, roundness};

            auto hex = color.Hex();
            const ui32 slot = PushTexture(m_circleTexture);
            std::vector<Vertex> cornerVertices;
            std::vector<ui32> cornerIndices = {
                     0, 1, 2,    1, 3, 2,
                     4, 5, 6,    5, 7, 6,
                     8, 9,10,    9,11,10,
                    12,13,14,   13,15,14,
                    16,17,18,   16,19,18,
                    20,21,22,   20,23,22
            };

            const float cos_r = std::cos(Math::DegToRad(rotation));
            const float sin_r = std::sin(Math::DegToRad(rotation));

            ui32 typeAndTex = slot | Vertex::TEXT;
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos,       cos_r, sin_r), {0.0f,0.0f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+rx,    cos_r, sin_r), {0.5f,0.0f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+ry,    cos_r, sin_r), {0.0f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+rx+ry, cos_r, sin_r), {0.5f,0.5f}, hex, typeAndTex});

            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+w-rx,   cos_r, sin_r), {0.5f,0.0f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+w,      cos_r, sin_r), {1.0f,0.0f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+w-rx+ry,cos_r, sin_r), {0.5f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+w+ry,   cos_r, sin_r), {1.0f,0.5f}, hex, typeAndTex});

            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+w+h-rx-ry, cos_r, sin_r), {0.5f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+w+h-ry,    cos_r, sin_r), {1.0f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+w+h-rx,    cos_r, sin_r), {0.5f,1.0f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+w+h,       cos_r, sin_r), {1.0f,1.0f}, hex, typeAndTex});

            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+h-ry,    cos_r, sin_r), {0.0f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+h+rx-ry, cos_r, sin_r), {0.5f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+h,       cos_r, sin_r), {0.0f,1.0f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos+h+rx,    cos_r, sin_r), {0.5f,1.0f}, hex, typeAndTex});


            typeAndTex = slot | Vertex::COLOR;
            // top rect
            auto pos2 = pos + rx;
            auto size = w+ry-rx-rx;
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos2,                     cos_r, sin_r), {0.0f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos2+Vector2f(size.x, 0), cos_r, sin_r), {0.5f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos2+size,                cos_r, sin_r), {0.5f,1.0f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos2+Vector2f(0, size.y), cos_r, sin_r), {0.0f,1.0f}, hex, typeAndTex});

            // bottom rect
            auto pos3 = pos + Vector2f(0,dimensions.y) + rx - ry;
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos3,                     cos_r, sin_r), {0.0f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos3+Vector2f(size.x, 0), cos_r, sin_r), {0.5f,0.5f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos3+size,                cos_r, sin_r), {0.5f,1.0f}, hex, typeAndTex});
            cornerVertices.push_back(Vertex{Math::Rotate(origin, pos3+Vector2f(0, size.y), cos_r, sin_r), {0.0f,1.0f}, hex, typeAndTex});

            PushQuad(pos+ry, dimensions-ry-ry, color, m_circleTexture, Vertex::COLOR, rotation);
            PushVertexData(cornerVertices, cornerIndices);
        }
    }

    void Renderer::DrawCircle(const Vector2f& pos, float radius, const Color& color) noexcept
    {
        PushQuad(pos - Vector2f{radius}, {radius * 2}, color, m_circleTexture, Vertex::TEXT);
    }

    void Renderer::DrawBezier(const Vector2f& a, const Vector2f& cp1, const Vector2f& cp2, const Vector2f& b, float thickness,
                              const Color& color, bool rounded, float step) noexcept
    {
        Vector2f begin = a;
        for(float i = 0; i < 1.f; i += step)
        {
            const Vector2f end = Math::Cubic(a,cp1,cp2,b,i);
            DrawLine(begin, end, thickness, color, rounded);
            begin = end;
        }
    }

    void Renderer::DrawBezier(const Vector2f& a, const Vector2f& cp, const Vector2f& b, float thickness,
                              const Color& color, bool rounded, float step) noexcept
    {
        Vector2f begin = a;
        float i = 0;
        while(i < 1)
        {
            const Vector2f end = Math::Quadratic(a,cp,b,i);
            DrawLine(begin, end, thickness, color, rounded);
            begin = end;
            i += step;
        }
    }

    void Renderer::DrawGrid(const Vector2f& top_left, const Vector2f& size, ui32 rows, ui32 columns, const Color& color,
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

    void Renderer::DrawTexture(const Texture& tex, const Vector2f& pos, const Vector2f& size) noexcept
    {
        PushQuad(pos, size, Color::Transparent, tex, Vertex::TEXTURE);
    }

    void Renderer::DrawTextureRect(const Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& tl, const Vector2f& br) noexcept
    {
        PushQuad(pos, size, Color::Transparent, tex, Vertex::TEXTURE, rotation, tl, br);
    }

    /// Finds a parking spot for the texture.
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

    void Renderer::PushQuad(const Vector2f& pos,
                            const Vector2f& size,
                            const Color& col,
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

        const ui32 typeAndTex = tex | type;
        if(rotation != 0)
        {
            const Vector2f origin = (pos + pos + size) * 0.5f;

            const float cos_r = std::cos(Math::DegToRad(rotation));
            const float sin_r = std::sin(Math::DegToRad(rotation));

            m_vertexData.push_back(Vertex{Math::Rotate(origin, pos, cos_r, sin_r), tl, hex, typeAndTex});
            m_vertexData.push_back(Vertex{Math::Rotate(origin, pos + Vector2f{size.x, 0.f}, cos_r, sin_r), {br.x, tl.y}, hex, typeAndTex});
            m_vertexData.push_back(Vertex{Math::Rotate(origin, pos + Vector2f{0.f, size.y}, cos_r, sin_r), {tl.x, br.y}, hex, typeAndTex});
            m_vertexData.push_back(Vertex{Math::Rotate(origin, pos + size, cos_r, sin_r), br, hex, typeAndTex});
        }
        else
        {
            m_vertexData.push_back(Vertex{pos, tl, hex, typeAndTex});
            m_vertexData.push_back(Vertex{pos + Vector2f{size.x, 0.f}, {br.x, tl.y}, hex, typeAndTex});
            m_vertexData.push_back(Vertex{pos + Vector2f{0.f, size.y}, {tl.x, br.y}, hex, typeAndTex});
            m_vertexData.push_back(Vertex{pos + size, br, hex, typeAndTex});
        }

        m_indexData.push_back(currentElements + 0);
        m_indexData.push_back(currentElements + 1);
        m_indexData.push_back(currentElements + 2);

        m_indexData.push_back(currentElements + 1);
        m_indexData.push_back(currentElements + 3);
        m_indexData.push_back(currentElements + 2);
    }

/// Windows annoyingly #defines DrawText
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
            i.texAndType = slot | i.texAndType;
        PushVertexData(vertices, indices);
    }

    void Renderer::EndBatch(bool flip) noexcept
    {
        m_shader->Bind();

        for(i32 i = 0; i < m_textures.size(); i++)
            m_shader->Uniform1i("uTexture" + std::to_string(i), i);

        if(flip) /// Flip the view vertically. This is for RenderTexture.
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

        if(flip) /// Flip the view vertically. This is for RenderTexture.
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