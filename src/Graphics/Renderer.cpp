#include "Renderer.h"
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>
#include <glm/gtc/matrix_transform.hpp>

#include <GLHeader.h>
#include <Shaders.h>
#include <MappedVector.h>
#include <_Assert.h>

#ifdef PLATFORM_WINDOWS
    #undef DrawText
#endif

namespace tml
{
    static constexpr void MakeCircle(Image& image, uint32_t resolution) noexcept
    {
        auto* buffer = image.GetData();
        const auto radius = resolution / 2.0;
        const auto center = Vector2f(radius);

        for(uint32_t i = 0; i < resolution; ++i)
        {
            for(uint32_t j = 0; j < resolution; ++j)
            {
                const double dist = Math::Distance(Vector2f(j, i), center);
                const double d = dist / radius;
                buffer[i * resolution + j] = static_cast<unsigned char>(Math::SmoothStep(0.0, 0.002, 1.0 - d) * 255.0);
            }
        }
    }

    Renderer::Renderer()
    {
        TML_ASSERT(gladLoaderLoadGL(), "Failed to load OpenGL functions");

#if !defined(TML_USE_GLES) && !defined(TML_NO_GL_DEBUGGING)
        GL_CALL(glad_glEnable(GL_DEBUG_OUTPUT));
        GL_CALL(glad_glDebugMessageCallback(GLMessageCallback, nullptr));
#endif

        GL_CALL(glad_glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureCount));

        m_maxTextureCount = Math::Min(m_maxTextureCount, 32);

        m_vao           = new VertexArray;
        m_vertexVector  = new VertexVector(s_maxVertexCount);
        m_indexVector   = new IndexVector(s_maxIndexCount);
        m_layout        = new BufferLayout;
        m_shader        = new Shader;
        m_text          = new Text;
        m_circleTexture = new Texture;

        m_vao->Bind();
        m_layout->Push(2, 4, BufferLayout::VERTEX_FLOAT);
        m_layout->Push(2, 4, BufferLayout::VERTEX_FLOAT);
        m_layout->Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);
        m_layout->Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);

        m_shader->LoadFromString(VERTEX_STRING, FRAGMENT_STRING);
        m_shader->Bind();

        Image circleImage;
        circleImage.LoadFromMemory(2048, 2048, 1, nullptr);
        MakeCircle(circleImage, 2048);
        m_circleTexture->LoadFromImage(circleImage);

        GL_CALL(glad_glEnable(GL_BLEND));
        GL_CALL(glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_CALL(glad_glBlendEquation(GL_FUNC_ADD));
    }

    Renderer::~Renderer()
    {
//        delete m_vao;
//        delete m_vertexVector;
//        delete m_indexVector;
//        delete m_layout;
//        delete m_shader;
    }

    Renderer& Renderer::GetInstance() noexcept
    {
        static Renderer s_rendererInstance;
        s_rendererInstance.Reset();
        return s_rendererInstance;
    }

    void Renderer::Reset() noexcept
    {
        *reinterpret_cast<glm::mat4*>(m_view)  = glm::mat4(1.0);
        *reinterpret_cast<glm::mat4*>(m_scale) = glm::mat4(1.0);
        *reinterpret_cast<glm::mat4*>(m_proj)  = glm::mat4(1.0);
    }

    void Renderer::SetClearColor(const Color &color) noexcept
    {
        m_clearColor[0] = float(color.r) / 255.0f;
        m_clearColor[1] = float(color.g) / 255.0f;
        m_clearColor[2] = float(color.b) / 255.0f;
        m_clearColor[3] = float(color.a) / 255.0f;
    }

    void Renderer::SetBounds(const Vector2i& pos, const Vector2i& size) noexcept
    {
        EndBatch();
        BeginBatch();
        GL_CALL(glad_glScissor(pos.x, m_viewport.size.y - pos.y - size.y, size.x, size.y));
        GL_CALL(glad_glEnable(GL_SCISSOR_TEST));
    }

    void Renderer::SetCamera(const Camera& camera) noexcept
    {
        EndBatch();
        BeginBatch();

        const auto pos = camera.GetPosition();
        const auto zoom = camera.GetZoom();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, camera.GetRotation(), glm::vec3(0.f, 0.f, 1.f));
        view = glm::scale(view, glm::vec3(zoom, zoom, 0));
        view = glm::translate(view, glm::vec3(-pos.x, -pos.y, 0));

        *reinterpret_cast<glm::mat4*>(m_view) = view;
    }

    void Renderer::ResetCamera() noexcept
    {
        EndBatch();
        BeginBatch();
        *reinterpret_cast<glm::mat4*>(m_view) = glm::mat4(1.0f);
    }

    void Renderer::SetViewport(const Vector2i &pos, const Vector2i &size) noexcept
    {
        EndBatch();
        BeginBatch();
        m_viewport = {pos, size};
        GL_CALL(glad_glViewport(pos.x, pos.y, size.x, size.y));
    }

    void Renderer::SetView(const Vector2i &pos, const Vector2i &size) noexcept
    {
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
        BeginBatch();
        GL_CALL(glad_glDisable(GL_SCISSOR_TEST));
    }

    void Renderer::Clear() noexcept
    {
        ResetBounds();
        GL_CALL(glad_glClearBufferfv(GL_COLOR, 0, m_clearColor));
    }

    void Renderer::Clear(float color[4]) noexcept
    {
        ResetBounds();
        GL_CALL(glad_glClearBufferfv(GL_COLOR, 0, color));
    }

    void Renderer::Draw(Drawable& d) noexcept
    {
        d.OnDraw(this, m_circleTexture);
    }

    void Renderer::DrawLine(const Vector2f &a, const Vector2f &b, float thickness, Color color, bool rounded) noexcept
    {
        uint32_t currentElements = m_vertexVector->size();
        if(currentElements >= s_maxVertexCount - 4)
        {
            EndBatch();
            currentElements = 0;
        }
        // dx and dy for normals
        const float dx = b.x - a.x;
        const float dy = b.y - a.y;
        const auto dirA = (Vector2f(-dy, dx).Normalized() * thickness * 0.5);
        const auto dirB = (Vector2f(dy, -dx).Normalized() * thickness * 0.5);

        m_vertexVector->push_back(Vertex{(dirA + a), {0, 0}, color.Hex(), Vertex::COLOR});
        m_vertexVector->push_back(Vertex{(dirB + a), {0, 0}, color.Hex(), Vertex::COLOR});
        m_vertexVector->push_back(Vertex{(dirA + b), {0, 0}, color.Hex(), Vertex::COLOR});
        m_vertexVector->push_back(Vertex{(dirB + b), {0, 0}, color.Hex(), Vertex::COLOR});

        m_indexVector->push_back(currentElements + 0);
        m_indexVector->push_back(currentElements + 1);
        m_indexVector->push_back(currentElements + 2);

        m_indexVector->push_back(currentElements + 1);
        m_indexVector->push_back(currentElements + 3);
        m_indexVector->push_back(currentElements + 2);

        if(rounded) /// Doesn't work well with translucent colors. Might do something to fix this in some point. TODO
        {
            Renderer::DrawCircle(a, thickness * 0.5f, color);
            Renderer::DrawCircle(b, thickness * 0.5f, color);
        }
    }

    void Renderer::DrawRect(const Vector2f& pos, const Vector2f& dimensions, const Color& color, float roundness, float rotation) noexcept
    {
        if(roundness < 1.f) // If roundness is too low, just draw a single quad
            PushQuad(pos, dimensions, color, *m_circleTexture, Vertex::COLOR, rotation);
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
            const uint32_t slot = PushTexture(*m_circleTexture);
            std::vector<Vertex> cornerVertices;
            std::vector<uint32_t> cornerIndices = {
                     0, 1, 2,    1, 3, 2,
                     4, 5, 6,    5, 7, 6,
                     8, 9,10,    9,11,10,
                    12,13,14,   13,15,14,
                    16,17,18,   16,19,18,
                    20,21,22,   20,23,22
            };

            const float cos_r = std::cos(Math::DegToRad(rotation));
            const float sin_r = std::sin(Math::DegToRad(rotation));

            uint32_t typeAndTex = slot | Vertex::TEXT;
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

            PushQuad(pos+ry, dimensions-ry-ry, color, *m_circleTexture, Vertex::COLOR, rotation);
            PushVertexData(cornerVertices, cornerIndices);
        }
    }

    void Renderer::DrawCircle(const Vector2f& pos, float radius, const Color& color) noexcept
    {
        PushQuad(pos - Vector2f{radius}, {radius * 2}, color, *m_circleTexture, Vertex::TEXT);
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

    void Renderer::DrawGrid(const Vector2f& top_left, const Vector2f& size, uint32_t rows, uint32_t columns, const Color& color,
                            float thickness, bool rounded) noexcept
    {
        for(uint32_t i = 0; i <= rows; ++i)
        {
            DrawLine(top_left + Vector2f{0.f,    (size.y / rows) * i},
                     top_left + Vector2f{size.x, (size.y / rows) * i}, thickness, color, ((i == 0) || (i == rows)) && rounded);
        }
        for(uint32_t i = 0; i <= columns; ++i)
        {
            DrawLine(top_left + Vector2f{(size.x / columns) * i, 0.f},
                     top_left + Vector2f{(size.x / columns) * i, size.y}, thickness, color, false);
        }
    }

    void Renderer::DrawTexture(const Texture& tex, const Vector2f& pos, const Vector2f& size) noexcept
    {
        PushQuad(pos, size, Color::Transparent, tex, Vertex::TEXTURE);
    }

    void Renderer::DrawTextureRect(const Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& tl, const Vector2f& br) noexcept
    {
        PushQuad(pos, size, Color::Transparent, tex, Vertex::TEXTURE, rotation, tl, br);
    }

    void Renderer::DrawText(const String& text, const Vector2f& pos, float size, const Color& color) noexcept
    {
        m_text->SetString(text);
        m_text->SetSize(size);
        m_text->SetColor(color);
        m_text->SetPosition(pos);
        Draw(*m_text);
    }

    void Renderer::PushVertexData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) noexcept
    {
        if(s_maxVertexCount <= m_vertexVector->size() + vertices.size())
        {
            EndBatch();
        }
        const auto size = m_vertexVector->size();
        m_vertexVector->PushData(vertices.data(), sizeof(Vertex), vertices.size());
        for(const auto i : indices)
            m_indexVector->push_back(size + i);
    }

    void Renderer::PushVertexData(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Texture& tex) noexcept
    {
        const uint32_t slot = PushTexture(tex);
        for(Vertex& i : vertices)
            i.texAndType = slot | i.texAndType;
        PushVertexData(vertices, indices);
    }

    /// Finds a parking spot for the texture.
    uint32_t Renderer::PushTexture(const Texture &texture) noexcept
    {
        if(m_textures.size() >= (size_t)m_maxTextureCount)
            EndBatch();

        bool alreadyInMTextures = false;
        const auto id = texture.GetID();
        uint32_t index = 0;

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
        uint32_t currentElements = m_vertexVector->size();
        if(currentElements >= s_maxVertexCount)
            EndBatch();

        const uint32_t tex = PushTexture(texture);
        const uint32_t hex = col.Hex();

        currentElements = m_vertexVector->size(); // PushTexture() might have ended the last batch, so we need to get the m_vertexVector->size() again

        const uint32_t typeAndTex = tex | type;
        if(rotation != 0)
        {
            const Vector2f origin = (pos + pos + size) * 0.5f;

            const float cos_r = std::cos(Math::DegToRad(rotation));
            const float sin_r = std::sin(Math::DegToRad(rotation));

            m_vertexVector->push_back(Vertex{Math::Rotate(origin, pos, cos_r, sin_r), tl, hex, typeAndTex});
            m_vertexVector->push_back(Vertex{Math::Rotate(origin, pos + Vector2f{size.x, 0.f}, cos_r, sin_r), {br.x, tl.y}, hex, typeAndTex});
            m_vertexVector->push_back(Vertex{Math::Rotate(origin, pos + Vector2f{0.f, size.y}, cos_r, sin_r), {tl.x, br.y}, hex, typeAndTex});
            m_vertexVector->push_back(Vertex{Math::Rotate(origin, pos + size, cos_r, sin_r), br, hex, typeAndTex});
        }
        else
        {
            m_vertexVector->push_back(Vertex{pos, tl, hex, typeAndTex});
            m_vertexVector->push_back(Vertex{pos + Vector2f{size.x, 0.f}, {br.x, tl.y}, hex, typeAndTex});
            m_vertexVector->push_back(Vertex{pos + Vector2f{0.f, size.y}, {tl.x, br.y}, hex, typeAndTex});
            m_vertexVector->push_back(Vertex{pos + size, br, hex, typeAndTex});
        }

        m_indexVector->push_back(currentElements + 0);
        m_indexVector->push_back(currentElements + 1);
        m_indexVector->push_back(currentElements + 2);

        m_indexVector->push_back(currentElements + 1);
        m_indexVector->push_back(currentElements + 3);
        m_indexVector->push_back(currentElements + 2);
    }

    void Renderer::BeginBatch() noexcept
    {
        m_textures.clear();
        m_vertexVector->BufferData(nullptr, sizeof(Vertex), s_maxVertexCount);
        m_indexVector->BufferData(nullptr, s_maxIndexCount);
    }

    void Renderer::EndBatch() noexcept
    {
        if(m_vertexVector->size() == 0)
            return;

        m_shader->Bind();

        for(int i = 0; i < static_cast<int>(m_textures.size()); ++i)
            m_shader->Uniform1i("uTexture" + std::to_string(i), i);

        m_shader->Uniform2f("uViewSize", m_viewport.size.x, m_viewport.size.y);
        m_shader->UniformMat4fv("uView",  1, false, m_view);
        m_shader->UniformMat4fv("uProj",  1, false, m_proj);
        m_shader->UniformMat4fv("uScale", 1, false, m_scale);

        m_vao->BufferData(*m_vertexVector, *m_indexVector, *m_layout);
        m_vao->Bind();
        m_vertexVector->Bind();
        m_indexVector->Bind();

        GL_CALL(glad_glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexVector->size()), GL_UNSIGNED_INT, nullptr));
        BeginBatch();
    }
}