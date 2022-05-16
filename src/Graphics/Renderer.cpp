#include "Renderer.h"
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <GLHeader.h>
#include <Shaders.h>
#include <_Assert.h>

#ifdef TML_PLATFORM_WINDOWS
    #undef DrawText
#endif

namespace tml
{

    Renderer::Renderer()
    {
#ifndef TML_USE_GLES
        TML_ASSERT(gladLoaderLoadGL(), "Failed to load OpenGL functions");
#else
        TML_ASSERT(gladLoadGLES2(glfwGetProcAddress), "Failed to load OpenGL ES functions");
#endif

#if !defined(TML_USE_GLES) && !defined(TML_NO_GL_DEBUGGING)
        GL_CALL(glad_glEnable(GL_DEBUG_OUTPUT));
        GL_CALL(glad_glDebugMessageCallback(GLMessageCallback, nullptr));
#endif
        GL_CALL(glad_glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureCount));

        m_maxTextureCount = Math::Min(m_maxTextureCount, 32);

        m_vao           = new VertexArray;
        m_vertexBuffer  = new VertexBuffer;
        m_indexBuffer   = new IndexBuffer;
        m_layout        = new BufferLayout;
        m_shader        = new Shader;

        m_textures.reserve(m_maxTextureCount);
        m_vertexData.reserve(s_maxVertexCount);
        m_indexData.reserve(s_maxIndexCount);

        m_vao->Bind();
        m_layout->Push(2, 4, BufferLayout::VERTEX_FLOAT);
        m_layout->Push(2, 4, BufferLayout::VERTEX_FLOAT);
        m_layout->Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);
        m_layout->Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);

        m_shader->LoadFromString(VERTEX_STRING, FRAGMENT_STRING);
        m_shader->Bind();


        GL_CALL(glad_glEnable(GL_BLEND));
        GL_CALL(glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_CALL(glad_glBlendEquation(GL_FUNC_ADD));
    }

    Renderer::~Renderer()
    {
        delete m_vao;
        delete m_vertexBuffer;
        delete m_indexBuffer;
        delete m_layout;
        delete m_shader;
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

    void Renderer::PushVertexData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) noexcept
    {
        if(CheckLimits(vertices.size(), indices.size(), 0))
        {
            EndBatch();
        }

        const auto size = m_vertexData.size();
        m_vertexData.insert(std::end(m_vertexData), std::begin(vertices), end(vertices));

        for(const auto i : indices)
            m_indexData.push_back(i + size);
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
        if(CheckLimits(0, 0, 1))
            EndBatch();

        const auto id = texture.GetID();
        const auto begin = std::begin(m_textures);
        const auto end = std::end(m_textures);
        const auto iterator = std::find(begin, end, id);
        const auto index = iterator - begin;

        texture.Bind(index);

        if(iterator == end)
            m_textures.push_back(id);

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
        if(CheckLimits(4, 6, 1))
            EndBatch();

        const uint32_t tex = PushTexture(texture);
        const uint32_t hex = col.Hex();
        const uint32_t typeAndTex = tex | type;
        const uint32_t currentElements = m_vertexData.size();

        if(rotation != 0)
        {
            const Vector2f origin = (pos + pos + size) * 0.5f;

            const float cos_r = std::cos(Math::DegToRad(rotation));
            const float sin_r = std::sin(Math::DegToRad(rotation));

            m_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos, cos_r, sin_r), tl, hex, typeAndTex});
            m_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + Vector2f{size.x, 0.f}, cos_r, sin_r), {br.x, tl.y}, hex, typeAndTex});
            m_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + Vector2f{0.f, size.y}, cos_r, sin_r), {tl.x, br.y}, hex, typeAndTex});
            m_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + size, cos_r, sin_r), br, hex, typeAndTex});
        }
        else
        {
            m_vertexData.emplace_back(Vertex{pos, tl, hex, typeAndTex});
            m_vertexData.emplace_back(Vertex{pos + Vector2f{size.x, 0.f}, {br.x, tl.y}, hex, typeAndTex});
            m_vertexData.emplace_back(Vertex{pos + Vector2f{0.f, size.y}, {tl.x, br.y}, hex, typeAndTex});
            m_vertexData.emplace_back(Vertex{pos + size, br, hex, typeAndTex});
        }

        m_indexData.emplace_back(currentElements + 0);
        m_indexData.emplace_back(currentElements + 1);
        m_indexData.emplace_back(currentElements + 2);

        m_indexData.emplace_back(currentElements + 1);
        m_indexData.emplace_back(currentElements + 3);
        m_indexData.emplace_back(currentElements + 2);
    }

    void Renderer::BeginBatch() noexcept
    {
        m_textures.clear();
        m_vertexData.clear();
        m_indexData.clear();
    }

    void Renderer::EndBatch() noexcept
    {
        if(m_vertexData.empty())
            return;

        m_shader->Bind();

        for(int i = 0; i < static_cast<int>(m_textures.size()); ++i)
            m_shader->Uniform1i("uTexture" + std::to_string(i), i);

        m_shader->Uniform2f("uViewSize", m_viewport.size.x, m_viewport.size.y);
        m_shader->UniformMat4fv("uView",  1, false, m_view);
        m_shader->UniformMat4fv("uProj",  1, false, m_proj);
        m_shader->UniformMat4fv("uScale", 1, false, m_scale);

        m_vertexBuffer->BufferData(m_vertexData.data(), sizeof(Vertex), m_vertexData.size());
        m_indexBuffer->BufferData(m_indexData.data(), m_indexData.size());

        m_vao->BufferData(*m_vertexBuffer, *m_indexBuffer, *m_layout);
        m_vao->Bind();
        m_vertexBuffer->Bind();
        m_indexBuffer->Bind();

        GL_CALL(glad_glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexBuffer->Elements()), GL_UNSIGNED_INT, nullptr));
        BeginBatch();
    }

    void Renderer::GetOpenGLVersion(int &major, int &minor) const noexcept
    {
        major = minor = 0;
        glad_glGetIntegerv(GL_MAJOR_VERSION, &major);
        glad_glGetIntegerv(GL_MINOR_VERSION, &minor);
    }

    inline bool Renderer::CheckLimits(uint32_t vertexCount, uint32_t indexCount, uint32_t textureCount) const noexcept
    {
        return (m_vertexData.size() + vertexCount)  > s_maxVertexCount ||
               (m_indexData.size()  + indexCount)   > s_maxIndexCount  ||
               (m_textures.size()   + textureCount) > m_maxTextureCount;
    }
}