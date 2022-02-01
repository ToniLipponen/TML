#include <TML/Graphics/Renderer.h>
#include <TML/Utilities/Utilities.h>

#include <GLHeader.h>

#include <algorithm>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <string>

#include "GlDebug.h"
#include "_Assert.h"
#include "Buffers.h"
#include "Shader.h"
#include "Shaders.h"
#include "DefaultText.h"
#include <Circle.h> // Circle texture data

extern tml::Text* DEFAULT_TEXT;
extern tml::Font* DEFAULT_FONT;

namespace tml
{
    static Texture*         s_circleTexture = nullptr;
    static VertexArray*     s_vao           = nullptr;
    static VertexBuffer*    s_vertexBuffer  = nullptr;
    static IndexBuffer*     s_indexBuffer   = nullptr;
    static Shader*          s_shader        = nullptr;
    static BufferLayout     s_layout;

    static glm::mat4 s_view = glm::mat4(1.f);
    static glm::mat4 s_proj = glm::mat4(1.f);
    static glm::mat4 s_scale = glm::mat4(1.f);
    static Vector2f  s_viewSize = {0, 0};
    static Camera s_camera;

    // Render batch related stuff
    constexpr static ui32 MAX_VERTEX_COUNT  = 100000;
    static i32 MAX_TEXTURE_COUNT = 8;

    static std::vector<Vertex>  s_vertexData;
    static std::vector<ui32>    s_indexData;
    static std::vector<ui32>    s_textures;

    void PrintInformation()
    {
        GL_CALL(glad_glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURE_COUNT));
        const GLubyte* vendor = glad_glGetString(GL_VENDOR);
        const GLubyte* renderer = glad_glGetString(GL_RENDERER);
        const GLubyte* version = glad_glGetString(GL_VERSION);

        i32 max_tex_size = 0, gpu_texture_units = 0;
        GL_CALL(glad_glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size));
        GL_CALL(glad_glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &gpu_texture_units));
        tml::Logger::InfoMessage("GPU: %s %s", vendor, renderer);
        tml::Logger::InfoMessage("OpenGL version: %s", version);
        tml::Logger::InfoMessage("GPU max texture size: %dx%d", max_tex_size, max_tex_size);
        tml::Logger::InfoMessage("GPU available texture units: %d", gpu_texture_units);
    }

    bool Renderer::Init() noexcept
    {
        const bool result = QuietInit();
        PrintInformation();
        return result;
    }

    bool Renderer::QuietInit() noexcept
    {
        s_vertexData.reserve(MAX_VERTEX_COUNT);
        s_indexData.reserve(MAX_VERTEX_COUNT * 1.5);

        int result = 0;
    #ifdef TML_USE_GLES
        result = gladLoadGLES2((GLADloadfunc)glfwGetProcAddress);
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

        s_vao           = new VertexArray();
        s_vertexBuffer  = new VertexBuffer(nullptr, sizeof(Vertex), MAX_VERTEX_COUNT);
        s_indexBuffer   = new IndexBuffer(nullptr, MAX_VERTEX_COUNT * 1.5);
        s_shader        = new Shader();
        s_circleTexture = new Texture();
        LoadDefaultText();

        s_vao->Bind();
        s_layout.Push(2, 4, BufferLayout::VERTEX_FLOAT);
        s_layout.Push(2, 4, BufferLayout::VERTEX_FLOAT);
        s_layout.Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);
        s_layout.Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);
        s_layout.Push(1, 4, BufferLayout::VERTEX_UNSIGNED_INT);

        s_shader->FromString(VERTEX_STRING, FRAGMENT_STRING);
        s_shader->Bind();

        Image circleImage;
        circleImage.LoadFromData(CIRCLE.data(), CIRCLE.size());
        s_circleTexture->LoadFromImage(circleImage);

        GL_CALL(glEnable(GL_BLEND));
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_CALL(glBlendEquation(GL_FUNC_ADD));

#ifndef TML_USE_GLES
    #ifndef TML_NO_GL_DEBUGGING
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLMessageCallback, nullptr);
    #endif
#endif
        return true;
    }

    void Renderer::Deinit() noexcept
    {
        delete s_circleTexture;
        delete s_vao;
        delete s_vertexBuffer;
        delete s_indexBuffer;
        delete s_shader;

        delete DEFAULT_TEXT;
        delete DEFAULT_FONT;
    }

    void Renderer::SetClearColor(const Color &color) noexcept
    {
        GL_CALL(glad_glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
    }

    void Renderer::SetCamera(Camera &cam) noexcept
    {
        ResetCamera();
        s_camera = cam;
        s_camera.Move((s_viewSize / 2) * -1);
        s_view = glm::rotate(s_view, cam.GetRotation(), glm::vec3(0.f, 0.f, 1.f));
        s_view = glm::scale(s_view, glm::vec3(cam.GetZoom(), cam.GetZoom(), 0));
        s_view = glm::translate(s_view, glm::vec3(-s_camera.GetPosition().x, -s_camera.GetPosition().y, 0));
    }

    void Renderer::ResetCamera() noexcept
    {
        EndBatch();
        s_view = glm::mat4(1.f);
        s_scale = glm::mat4(1.f);
    }

    void Renderer::SetBounds(const Vector2i& pos, const Vector2i& size) noexcept
    {
        EndBatch();
        GL_CALL(glad_glScissor(pos.x, s_viewSize.y - pos.y - size.y, size.x, size.y));
        GL_CALL(glad_glEnable(GL_SCISSOR_TEST));
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

        int f[4];
        GL_CALL(glad_glGetIntegerv(GL_VIEWPORT, f));
        s_viewSize = Vector2f{static_cast<float>(f[2]), static_cast<float>(f[3])};
        s_proj = glm::ortho(
                static_cast<float>(f[0]),
                static_cast<float>(f[2]),
                static_cast<float>(f[3]),
                static_cast<float>(f[1])
        );
        ResetCamera();
        ResetBounds();
    }

    void Renderer::Draw(Rectangle& r) noexcept
    {
        PushVertexData(r.m_vertexData, r.m_indexData);
    }

    void Renderer::Draw(Circle& r) noexcept
    {
        PushVertexData(r.m_vertexData, r.m_indexData, *s_circleTexture);
    }

    void Renderer::Draw(Sprite& r) noexcept
    {
        PushVertexData(r.m_vertexData, r.m_indexData, r.m_tex);
    }

    void Renderer::Draw(Text& r) noexcept
    {
        PushVertexData(r.m_vertexData, r.m_indexData, r.m_font.m_texture);
    }

    void Renderer::Draw(Video& r) noexcept
    {
        if(s_textures.size() >= MAX_TEXTURE_COUNT - 3)
            EndBatch();
        PushQuad(r.m_pos, r.m_size, r.m_color, r.m_y, Vertex::VIDEO);
        PushTexture(r.m_cb);
        PushTexture(r.m_cr);
    }

    void Renderer::DrawLine(const Vector2f &a, const Vector2f &b, float thickness, Color color, bool rounded) noexcept
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
        thickness = ceilf(thickness);

        s_vertexData.push_back({((Vector2f(-dy, dx).Normalized() * thickness * 0.5) + a), {0, 0}, color.Hex(), 0, Vertex::COLOR});
        s_vertexData.push_back({((Vector2f(dy, -dx).Normalized() * thickness * 0.5) + a), {0, 0}, color.Hex(), 0, Vertex::COLOR});
        s_vertexData.push_back({((Vector2f(-dy, dx).Normalized() * thickness * 0.5) + b), {0, 0}, color.Hex(), 0, Vertex::COLOR});
        s_vertexData.push_back({((Vector2f(dy, -dx).Normalized() * thickness * 0.5) + b), {0, 0}, color.Hex(), 0, Vertex::COLOR});

        s_indexData.push_back(currentElements + 0);
        s_indexData.push_back(currentElements + 1);
        s_indexData.push_back(currentElements + 2);

        s_indexData.push_back(currentElements + 1);
        s_indexData.push_back(currentElements + 3);
        s_indexData.push_back(currentElements + 2);

        if(rounded) // Doesn't work well with translucent colors. Might do something to fix this in some point. TODO
        {
            Renderer::DrawCircle(a, thickness * 0.5f, color);
            Renderer::DrawCircle(b, thickness * 0.5f, color);
        }
    }

    void Renderer::DrawRect(const Vector2f& pos, const Vector2f& dimensions, const Color& color, float roundness, float rotation) noexcept
    {
        if(roundness < 3.f) // If roundness is too low, just draw a single quad
            PushQuad(pos, dimensions, color, *s_circleTexture, Vertex::COLOR, rotation);
        else
        {
            // Doesn't work well with translucent colors. Might do something to fix this in some point. TODO
            const Vector2f origin = {(pos.x + pos.x + dimensions.x) * 0.5f,
                                     (pos.y + pos.y + dimensions.y) * 0.5f};
            PushQuad(pos+Vector2f{0.f, roundness}, dimensions - Vector2f{0.f, roundness*2}, color, *s_circleTexture, Vertex::COLOR, rotation);
            PushQuad(pos+Vector2f{roundness, 0.f}, dimensions - Vector2f{roundness*2, 0.f}, color, *s_circleTexture, Vertex::COLOR, rotation);

            DrawCircle(Math::Rotate(origin, pos+Vector2f{roundness, roundness}, rotation), roundness, color);
            DrawCircle(Math::Rotate(origin, pos+Vector2f{dimensions.x - roundness, roundness}, rotation), roundness, color);
            DrawCircle(Math::Rotate(origin,pos+Vector2f{roundness,dimensions.y - roundness}, rotation), roundness, color);
            DrawCircle(Math::Rotate(origin,pos+dimensions-Vector2f{roundness, roundness}, rotation), roundness, color);
        }
    }

    void Renderer::DrawCircle(const Vector2f& pos, float radius, const Color& color) noexcept
    {
        PushQuad(pos - Vector2f{radius}, {radius * 2}, color, *s_circleTexture, Vertex::TEXT);
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

    void Renderer::DrawTexture(Texture &tex, const Vector2f &pos, const Vector2f &size) noexcept
    {
        PushQuad(pos, size, TRANSPARENT, tex, Vertex::TEXTURE);
    }

    void Renderer::DrawTextureRect(Texture& tex, const Vector2f& pos, const Vector2f& size, float rotation, const Vector2f& tl, const Vector2f& br) noexcept
    {
        PushQuad(pos, size, TRANSPARENT, tex, Vertex::TEXTURE, rotation, tl, br);
    }

    void Renderer::PushQuad(const Vector2f &pos,
                            const Vector2f &size,
                            const Color &col,
                            Texture& texture,
                            Vertex::DrawableType type,
                            float rotation,
                            const Vector2f& tl,
                            const Vector2f& br) noexcept
    {
        ui32 currentElements = s_vertexData.size();
        if(currentElements >= MAX_VERTEX_COUNT - 4)
            EndBatch();

        const ui32 tex = PushTexture(texture);
        const ui32 hex = col.Hex();

        currentElements = s_vertexData.size(); // PushTexture() might have ended the last batch, so we need to get the s_vertexData.size() again

        if(rotation != 0)
        {
            const Vector2f origin = (pos + pos + size) * 0.5f;
            s_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos, rotation),                         tl, hex, tex, type});
            s_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + Vector2f{size.x, 0.f}, rotation), {br.x, tl.y}, hex, tex, type});
            s_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + Vector2f{0.f, size.y}, rotation), {tl.x, br.y}, hex, tex, type});
            s_vertexData.emplace_back(Vertex{Math::Rotate(origin, pos + size, rotation),                  br, hex, tex, type});
        }
        else
        {
            s_vertexData.emplace_back(Vertex{pos,                         tl, hex, tex, type});
            s_vertexData.emplace_back(Vertex{pos + Vector2f{size.x, 0.f}, {br.x, tl.y}, hex, tex, type});
            s_vertexData.emplace_back(Vertex{pos + Vector2f{0.f, size.y}, {tl.x, br.y}, hex, tex, type});
            s_vertexData.emplace_back(Vertex{pos + size,                  br, hex, tex, type});
        }

        s_indexData.emplace_back(currentElements + 0);
        s_indexData.emplace_back(currentElements + 1);
        s_indexData.emplace_back(currentElements + 2);

        s_indexData.emplace_back(currentElements + 1);
        s_indexData.emplace_back(currentElements + 3);
        s_indexData.emplace_back(currentElements + 2);
    }

    void Renderer::DrawText(const std::string &text, const Vector2f &pos, float size, const Color &color) noexcept
    {
        DEFAULT_TEXT->SetString(text);
        DEFAULT_TEXT->SetSize(size);
        DEFAULT_TEXT->SetColor(color);
        DEFAULT_TEXT->SetPosition(pos);
        Draw(*DEFAULT_TEXT);
    }

    // Finds a parking spot for the texture.
    ui32 Renderer::PushTexture(Texture &texture) noexcept
    {
        if(s_textures.size() >= MAX_TEXTURE_COUNT)
            EndBatch();

        bool alreadyInMTextures = false;
        const auto id = texture.GetID();
        ui32 index = 0;

        for(const auto i : s_textures)
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
            s_textures.push_back(id);
        }

        return index;
    }
    
    void Renderer::PushVertexData(std::vector<Vertex>& vertices, std::vector<ui32>& indices) noexcept
    {
        if(MAX_VERTEX_COUNT <= s_vertexData.size() + vertices.size())
            EndBatch();
        const auto size = s_vertexData.size();
        std::copy(vertices.begin(), vertices.end(), std::back_inserter(s_vertexData));
        for(const auto i : indices)
            s_indexData.push_back(size + i);
    }

    void Renderer::PushVertexData(std::vector<Vertex>& vertices, std::vector<ui32>& indices, Texture& tex) noexcept
    {
        const ui32 slot = PushTexture(tex);
        for(Vertex& i : vertices)
            i.tex = slot;
        PushVertexData(vertices, indices);
    }

    void Renderer::EndBatch() noexcept
    {
        s_shader->Bind();

        for(i32 i = 0; i < MAX_TEXTURE_COUNT; i++)
            s_shader->Uniform1i("uTexture" + std::to_string(i), i);

        s_shader->SetVec2("uViewSize", s_viewSize);
        s_shader->UniformMat4fv("uView",  1, false, &s_view [0][0]);
        s_shader->UniformMat4fv("uProj",  1, false, &s_proj [0][0]);
        s_shader->UniformMat4fv("uScale", 1, false, &s_scale[0][0]);

        s_vertexBuffer->PushData(s_vertexData.data(), sizeof(Vertex), s_vertexData.size());
        s_indexBuffer->PushData(s_indexData.data(), s_indexData.size());
        s_vao->BufferData(*s_vertexBuffer, *s_indexBuffer, s_layout);

        s_vao->Bind();
        s_vertexBuffer->Bind();
        s_indexBuffer->Bind();

        GL_CALL(glad_glDrawElements(GL_TRIANGLES, s_indexBuffer->Elements(), GL_UNSIGNED_INT, nullptr));
        Renderer::BeginBatch();
    }

    void Renderer::BeginBatch() noexcept
    {
        s_vertexData.clear();
        s_indexData.clear();
        for(auto& i : s_textures)
            i = -1;
        s_textures.clear();
        s_vertexBuffer->Flush();
        s_indexBuffer->Flush();
    }
}