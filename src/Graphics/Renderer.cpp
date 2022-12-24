#include "Renderer.h"
#include <TML/Graphics/Core/Buffers.h>
#include <TML/Graphics/Core/Shader.h>
#include <sstream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <GLHeader.h>
#include <Shaders.h>
#include <_Assert.h>

#ifdef TML_PLATFORM_WINDOWS
    #undef DrawText
#endif
static std::string s_defaultFragmentSource;

void CreateDefaultFragmentShader(int textureSlots)
{
    s_defaultFragmentSource =
    R"END(
    #version 300 es
    in mediump vec4 vColor;
    in mediump vec2 vUV;
    flat in uint vTexID;
    flat in uint vType;
    layout (location = 0) out mediump vec4 outColor;

    )END";

    for(int i = 0; i < textureSlots; i++)
    {
        s_defaultFragmentSource.append("uniform sampler2D uTexture" + std::to_string(i) + ";\n");
    }

    s_defaultFragmentSource.append(
            R"END(
    mediump vec4 SampleTex(uint index)
    {
        switch(index)
        {

    )END");

    for(int i = 0; i < textureSlots; i++)
    {
        std::stringstream ss;
        ss << "case " << i << "u: return texture(uTexture" << i << ", vUV); break;\n";
        s_defaultFragmentSource.append(ss.str());
    }

    s_defaultFragmentSource.append(R"END(
        default: return vec4(1.0, 0.0, 0.0, 1.0); break;
        }
    }
    )END");

    s_defaultFragmentSource.append(R"END(
    void main()
    {
       switch(vType)
       {
           /// Color
           case 0u:
               outColor = vColor;
           break;

           /// Texture
           case 1u:
               outColor = SampleTex(vTexID);
               if(outColor.a < 0.01)
                   discard;
           break;

           /// Text
           case 2u:
               mediump vec4 color = SampleTex(vTexID);
               
               mediump float w = fwidth(color.r);
               mediump float alpha = smoothstep(0.5-w, 0.5+w, color.r) * vColor.a;
               outColor = vColor;
               outColor.a = alpha;
           break;

           default:
           {
               discard;
           } break;
       }
    }
)END");
}

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
        CreateDefaultFragmentShader(m_maxTextureCount);

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

        m_shader->LoadFromString(VERTEX_STRING, s_defaultFragmentSource);
        m_shader->Bind();

        GL_CALL(glad_glEnable(GL_BLEND));
        GL_CALL(glad_glEnable(GL_DEPTH_TEST));
        GL_CALL(glad_glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
        // GL_CALL(glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_CALL(glad_glDepthFunc(GL_LEQUAL));
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
        m_currentState = {};
        m_currentState.viewMatrix = Matrix4f::Identity();
        m_currentState.projectionMatrix = Matrix4f::Identity();
    }

    void Renderer::SetClearColor(const Color &color) noexcept
    {
        m_currentState.clearColor[0] = float(color.r) / 255.0f;
        m_currentState.clearColor[1] = float(color.g) / 255.0f;
        m_currentState.clearColor[2] = float(color.b) / 255.0f;
        m_currentState.clearColor[3] = float(color.a) / 255.0f;
    }

    void Renderer::SetBounds(const Vector2i& pos, const Vector2i& size) noexcept
    {
        EndBatch();
        BeginBatch();

        const Vector2i viewPos = m_currentState.viewOffsetAndDimensions.pos;
        const Vector2i viewportSize = m_currentState.viewport.size;
        const int32_t y = viewportSize.y - pos.y - size.y + viewPos.y;
        const int32_t x = pos.x - static_cast<int32_t>(viewPos.x);

        GL_CALL(glad_glEnable(GL_SCISSOR_TEST));
        GL_CALL(glad_glScissor(x, y, size.x, size.y));
        m_currentState.scissor = {{x,y}, size};
    }

    void Renderer::SetCamera(const Camera& camera) noexcept
    {
        EndBatch();
        BeginBatch();
        m_currentState.viewMatrix = camera.GetMatrix() *
                Matrix4f::Translate({m_currentState.viewport.size.x / 2, m_currentState.viewport.size.y / 2, 0});
    }

    void Renderer::ResetCamera() noexcept
    {
        EndBatch();
        BeginBatch();
        m_currentState.viewMatrix = Matrix4f::Identity();
    }

    void Renderer::SetViewport(const Vector2i& pos, const Vector2i& size) noexcept
    {
        EndBatch();
        BeginBatch();
        m_currentState.viewport = {pos, size};
        GL_CALL(glad_glViewport(pos.x, pos.y, size.x, size.y));
    }

    void Renderer::SetView(const Vector2i& pos, const Vector2i& size) noexcept
    {
		const Vector2f fPos = pos;
		const Vector2f fSize = size;

        m_currentState.viewOffsetAndDimensions.pos = fPos;
        m_currentState.viewOffsetAndDimensions.size = fSize;

        m_currentState.projectionMatrix = Matrix4f::Orthographic(fPos.x,
                                                                 fPos.x + fSize.x,
                                                                 fPos.y,
                                                                 fPos.y + fSize.y,
                                                                 0.f,
                                                                 10000.f);
    }

    void Renderer::ResetBounds() noexcept
    {
        EndBatch();
        BeginBatch();
        GL_CALL(glad_glDisable(GL_SCISSOR_TEST));
        m_currentState.scissor = {};
    }

    void Renderer::Clear() noexcept
    {
        constexpr float f = std::numeric_limits<float>::max();
        ResetBounds();
        GL_CALL(glad_glClearBufferfv(GL_COLOR, 0, (float*)&m_currentState.clearColor));
        GL_CALL(glad_glClearBufferfv(GL_DEPTH, 0, &f));
    }

    void Renderer::Clear(float color[4]) noexcept
    {
        float f = std::numeric_limits<float>::max();
        ResetBounds();

        GL_CALL(glad_glClearBufferfv(GL_COLOR, 0, color));
        GL_CALL(glad_glClearBufferfv(GL_DEPTH, 0, &f));
    }

    void Renderer::PushVertexData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) noexcept
    {
        if(CheckLimits(vertices.size(), indices.size(), 0))
        {
            EndBatch();
        }

        const auto size = m_vertexData.size();
        m_vertexData.insert(std::end(m_vertexData), std::begin(vertices), std::end(vertices));

        for(const auto i : indices)
        {
            m_indexData.push_back(i + size);
        }
    }

    void Renderer::PushVertexData(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Texture& tex) noexcept
    {
        const uint32_t slot = PushTexture(tex);

        for(Vertex& i : vertices)
        {
            i.typeAndTexture = (slot << 16) | (i.typeAndTexture & 0xff000000) | (i.typeAndTexture & 0x0000ff00);
        }

        PushVertexData(vertices, indices);
    }

    /// Finds a parking spot for the texture.
    uint32_t Renderer::PushTexture(const Texture &texture) noexcept
    {
        const auto id = texture.GetID();
        const auto begin = std::begin(m_textures);
        const auto end = std::end(m_textures);
        const auto iterator = std::find(begin, end, id);
        const auto index = iterator - begin;

        if(iterator == end)
        {
            if(CheckLimits(0, 0, 1))
            {
                EndBatch();
            }

            m_textures.push_back(id);
            texture.Bind(index);
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
        if(CheckLimits(4, 6, 1))
        {
            EndBatch();
        }

        const uint32_t tex = PushTexture(texture);
        const uint32_t hex = col.Hex();
        const uint32_t currentElements = m_vertexData.size();

        const Vector2f origin = (pos + pos + size) * 0.5f;

        const auto cos_r = static_cast<float>(std::cos(Math::DegreesToRadians(rotation)));
        const auto sin_r = static_cast<float>(std::sin(Math::DegreesToRadians(rotation)));

        m_vertexData.emplace_back(
                Vertex{Math::Rotate(origin, pos, cos_r, sin_r), tl, hex, type, tex});
        m_vertexData.emplace_back(
                Vertex{Math::Rotate(origin, pos + Vector2f{size.x, 0.f}, cos_r, sin_r), {br.x, tl.y}, hex, type, tex});
        m_vertexData.emplace_back(
                Vertex{Math::Rotate(origin, pos + Vector2f{0.f, size.y}, cos_r, sin_r), {tl.x, br.y}, hex, type, tex});
        m_vertexData.emplace_back(
                Vertex{Math::Rotate(origin, pos + size, cos_r, sin_r), br, hex, type, tex});

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
        {
            return;
        }

        m_shader->Bind();

        for(int i = 0; i < static_cast<int>(m_textures.size()); ++i)
        {
            m_shader->Uniform1i("uTexture" + std::to_string(i), i);
        }

        m_shader->UniformMat4fv("uView", 1, false, &m_currentState.viewMatrix[0][0]);
        m_shader->UniformMat4fv("uProj", 1, true, &m_currentState.projectionMatrix[0][0]);

        m_vertexBuffer->BufferData(m_vertexData.data(), sizeof(Vertex), m_vertexData.size());
        m_indexBuffer->BufferData(m_indexData.data(), m_indexData.size());

        m_vao->BufferData(*m_vertexBuffer, *m_indexBuffer, *m_layout);
        m_vao->Bind();
        m_vertexBuffer->Bind();
        m_indexBuffer->Bind();

        GL_CALL(glad_glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexBuffer->Elements()), GL_UNSIGNED_INT, nullptr));
        BeginBatch();
    }

    void Renderer::GetOpenGLVersion(int& major, int& minor) const noexcept
    {
        major = minor = 0;
        GL_CALL(glad_glGetIntegerv(GL_MAJOR_VERSION, &major));
        GL_CALL(glad_glGetIntegerv(GL_MINOR_VERSION, &minor));
    }

	const Matrix4f& Renderer::GetViewMatrix() const noexcept
	{
		return m_currentState.viewMatrix;
	}

    void Renderer::PushState() noexcept
    {
        EndBatch();
        BeginBatch();
        m_statesStack.push(m_currentState);
    }

    void Renderer::PopState() noexcept
    {
        if(!m_statesStack.empty())
        {
            const bool oldScissor = m_currentState.scissor.size != Vector2i(0,0);

            m_currentState = m_statesStack.top();
            m_statesStack.pop();

            if(m_currentState.scissor.size != Vector2i(0,0))
            {
                SetBounds(m_currentState.scissor.pos, m_currentState.scissor.size);
            }
            else if(oldScissor)
            {
                ResetBounds();
            }

            SetViewport(m_currentState.viewport.pos, m_currentState.viewport.size);
        }
    }

    inline bool Renderer::CheckLimits(uint32_t vertexCount, uint32_t indexCount, uint32_t textureCount) const noexcept
    {
        return (m_vertexData.size() + vertexCount)  > s_maxVertexCount ||
               (m_indexData.size()  + indexCount)   > s_maxIndexCount  ||
               (m_textures.size()   + textureCount) > static_cast<uint32_t>(m_maxTextureCount);
    }
}