#include <TML/Renderer.h>
#include <TML/Utilities/Utilities.h>

#include <GLHeader.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <string>

#include "internal/GlDebug.h"
#include "internal/_Assert.h"
#include "internal/Buffers.h"
#include "internal/Shader.h"
#include "internal/Shaders.h"

#include <Circle.h> // Circle texture data

extern tml::Text* DEFAULT_TEXT;

namespace tml
{
    static Texture*         s_circleTexture = nullptr;
    static VertexArray*     s_vao = nullptr;
    static VertexBuffer*    s_vertexBuffer = nullptr;
    static IndexBuffer*     s_indexBuffer = nullptr;
    static BufferLayout     s_layout;
    static Shader*          s_shader = nullptr;

    static glm::mat4 s_view = glm::mat4(1.f);
    static glm::mat4 s_proj = glm::mat4(1.f);
    static glm::mat4 s_scale = glm::mat4(1.f);
    static Vector2  s_viewSize = {0, 0};
    static Camera s_camera;

    // Render batch related stuff
    constexpr static ui32 MAX_VERTEX_COUNT  = 1000000;
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
    #ifndef TML_USE_GLES
    void GLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
    {
        auto const src_str = [source]() {
            switch (source)
            {
                case GL_DEBUG_SOURCE_API: return "API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
                case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
                case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
                case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
                case GL_DEBUG_SOURCE_OTHER: return "OTHER";
            }
            return "UNKNOWN";
        }();

        auto const type_str = [type]() {
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR: return "ERROR";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
                case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
                case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
                case GL_DEBUG_TYPE_MARKER: return "MARKER";
                case GL_DEBUG_TYPE_OTHER: return "OTHER";
            }
            return "UNKNOWN";
        }();

        auto const severity_str = [severity]() {
            switch (severity) {
                case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
                case GL_DEBUG_SEVERITY_LOW: return "LOW";
                case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
                case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
            }
            return "UNKNOWN";
        }();

        std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
    }
    #endif

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
        DEFAULT_TEXT    = new Text();
        s_circleTexture = new Texture();

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
        s_circleTexture->SetMinMagFilter(Texture::Linear, Texture::Linear);
        s_circleTexture->LoadFromImage(circleImage);

        GL_CALL(glEnable(GL_BLEND));
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_CALL(glBlendEquation(GL_FUNC_ADD));
        GL_CALL(glDisable(GL_CULL_FACE));

        #ifndef TML_USE_GLES
            #ifndef TML_NO_GL_DEBUGGING
                glEnable(GL_DEBUG_OUTPUT);
                glDebugMessageCallback(GLMessageCallback, nullptr);
            #endif
        #endif
        return true;
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

//    void Renderer::SetViewport(i32 x, i32 y, ui32 w, ui32 h) noexcept
//    {
//        EndBatch();
//        s_viewSize = Vector2{static_cast<float>(w), static_cast<float>(h)};
//        s_proj = glm::ortho(
//                static_cast<float>(x),
//                static_cast<float>(w),
//                static_cast<float>(h),
//                static_cast<float>(y)
//        );
//    }
//
//    void Renderer::SetViewport(const Vector2& pos, const Vector2& size) noexcept
//    {
//        EndBatch();
//        s_viewSize = size;
//        s_proj = glm::ortho(
//                pos.x,
//                size.x,
//                size.y,
//                pos.y
//        );
//    }
//
//    void Renderer::ResetViewport() noexcept
//    {
//        int f[4];
//        GL_CALL(glad_glGetIntegerv(GL_VIEWPORT, f));
//        s_viewSize = Vector2{static_cast<float>(f[2]), static_cast<float>(f[3])};
//        s_proj = glm::ortho(
//                static_cast<float>(f[0]),
//                static_cast<float>(f[2]),
//                static_cast<float>(f[3]),
//                static_cast<float>(f[1])
//        );
//    }

    void Renderer::SetBounds(const Vector2& pos, const Vector2& size) noexcept
    {
        EndBatch();
        glad_glScissor(pos.x, s_viewSize.y - pos.y - size.y, size.x, size.y);
        glad_glEnable(GL_SCISSOR_TEST);
    }

    void Renderer::ResetBounds() noexcept
    {
        EndBatch();
        glad_glDisable(GL_SCISSOR_TEST);
    }

    void Renderer::Clear() noexcept
    {
        GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT));
        GL_CALL(BeginBatch());

        int f[4];
        GL_CALL(glad_glGetIntegerv(GL_VIEWPORT, f));
        s_viewSize = Vector2{static_cast<float>(f[2]), static_cast<float>(f[3])};
        s_proj = glm::ortho(
                static_cast<float>(f[0]),
                static_cast<float>(f[2]),
                static_cast<float>(f[3]),
                static_cast<float>(f[1])
        );
//        ResetViewport();
        ResetCamera();
        ResetBounds();
    }

    void Renderer::Draw(Rectangle& r) noexcept
    {
        PushQuad(r.m_pos, r.m_size, r.m_color, r.m_tex, r.m_rotation, Vertex::RECTANGLE);
    }

    void Renderer::Draw(Circle& r) noexcept
    {
        PushQuad(r.m_pos - (r.m_size * 0.5f), r.m_size, r.m_color, *s_circleTexture, Vertex::CIRCLE);
    }

    void Renderer::Draw(Sprite& r) noexcept
    {
        ui32 currentElements = s_vertexData.size();
        if((currentElements + 4) >= MAX_VERTEX_COUNT)
        {
            EndBatch();
            currentElements = 0;
        }

        ui32 tex = 0;
        if(r.m_tex.GetID() != UINT_MAX)
            tex = PushTexture(r.m_tex);
        else
            return;

        currentElements = s_vertexData.size(); // PushTexture() might have ended the last batch, so we need to get the s_vertexData.size() again
        Vector2 tl = r.m_rect.pos / r.m_texSize;
        Vector2 br = (r.m_rect.pos + r.m_rect.size) / r.m_texSize;
        const Vector2 origin = (r.m_pos + r.m_pos + r.m_size) * 0.5f;
        s_vertexData.emplace_back(Vertex{Util::Rotate(origin, r.m_pos, r.m_rotation),                            tl, r.m_color.Hex(), tex, Vertex::TEXTURE});
        s_vertexData.emplace_back(Vertex{Util::Rotate(origin, r.m_pos + Vector2{r.m_size.x, 0.f}, r.m_rotation), {br.x, tl.y}, r.m_color.Hex(), tex, Vertex::TEXTURE});
        s_vertexData.emplace_back(Vertex{Util::Rotate(origin, r.m_pos + Vector2{0.f, r.m_size.y}, r.m_rotation), {tl.x, br.y}, r.m_color.Hex(), tex, Vertex::TEXTURE});
        s_vertexData.emplace_back(Vertex{Util::Rotate(origin, r.m_pos + r.m_size, r.m_rotation),                 br, r.m_color.Hex(), tex, Vertex::TEXTURE});

        s_indexData.emplace_back(currentElements + 0);
        s_indexData.emplace_back(currentElements + 1);
        s_indexData.emplace_back(currentElements + 2);

        s_indexData.emplace_back(currentElements + 1);
        s_indexData.emplace_back(currentElements + 3);
        s_indexData.emplace_back(currentElements + 2);
    //    PushQuad(r.m_pos, r.m_size, r.m_color, r.m_tex, r.m_rotation, Vertex::TEXTURE);
    }

    void Renderer::Draw(Text& r) noexcept
    {
        ui32 currentElements = s_vertexData.size();
        if((currentElements + r.m_vertexData.size()) >= MAX_VERTEX_COUNT)
        {
            EndBatch();
            currentElements = 0;
        }

        ui32 tex = 0;
        if(r.m_font.m_texture.GetID() != UINT_MAX)
        {
            tex = PushTexture(r.m_font.m_texture);
        }
        else {
            return;
        }
        currentElements = s_vertexData.size(); // PushTexture() might have ended the last batch, so we need to get the s_vertexData.size() again

        for(auto& v : r.m_vertexData)
        {
            v.tex = tex;
            s_vertexData.push_back(v);
        }
        for(auto i : r.m_indexData)
            s_indexData.push_back(currentElements + i);
    }

    void Renderer::Draw(Video& r) noexcept
    {
        if(s_textures.size() >= MAX_TEXTURE_COUNT - 3)
            EndBatch();
        PushQuad(r.m_pos, r.m_size, r.m_color, r.m_y, Vertex::VIDEO);
        PushTexture(r.m_cb);
        PushTexture(r.m_cr);
    }

    void Renderer::DrawLine(const Vector2 &a, const Vector2 &b, float thickness, Color color, bool rounded) noexcept
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

        s_vertexData.push_back({((Vector2(-dy, dx).Normalized() * thickness * 0.5) + a), {0, 0}, color.Hex(), 0, Vertex::RECTANGLE});
        s_vertexData.push_back({((Vector2(dy, -dx).Normalized() * thickness * 0.5) + a), {0, 0}, color.Hex(), 0, Vertex::RECTANGLE});
        s_vertexData.push_back({((Vector2(-dy, dx).Normalized() * thickness * 0.5) + b), {0, 0}, color.Hex(), 0, Vertex::RECTANGLE});
        s_vertexData.push_back({((Vector2(dy, -dx).Normalized() * thickness * 0.5) + b), {0, 0}, color.Hex(), 0, Vertex::RECTANGLE});

        s_indexData.push_back(currentElements + 0);
        s_indexData.push_back(currentElements + 1);
        s_indexData.push_back(currentElements + 2);

        s_indexData.push_back(currentElements + 1);
        s_indexData.push_back(currentElements + 3);
        s_indexData.push_back(currentElements + 2);

        if(rounded)
        {
            Renderer::DrawCircle(a, thickness * 0.5f, color);
            Renderer::DrawCircle(b, thickness * 0.5f, color);
        }
    }

    void Renderer::DrawRect(const Vector2& pos, const Vector2& dimensions, const Color& color, float roundness, float rotation) noexcept
    {
        if(roundness < 3.f) // If roundness is too low, just draw a regular rectangle
            PushQuad(pos, dimensions, color, *s_circleTexture, rotation, Vertex::RECTANGLE);
        else
        {
            Vector2 origin = {origin.x = (pos.x + pos.x + dimensions.x) * 0.5f,
                              origin.y = (pos.y + pos.y + dimensions.y) * 0.5f};
            PushQuad(pos+Vector2{0.f, roundness}, dimensions - Vector2{0.f, roundness*2}, color, *s_circleTexture, rotation, Vertex::RECTANGLE);
            PushQuad(pos+Vector2{roundness, 0.f}, dimensions - Vector2{roundness*2, 0.f}, color, *s_circleTexture, rotation, Vertex::RECTANGLE);

            DrawCircle(Util::Rotate(origin, pos+Vector2{roundness, roundness}, rotation), roundness, color);
            DrawCircle(Util::Rotate(origin, pos+Vector2{dimensions.x - roundness, roundness}, rotation), roundness, color);
            DrawCircle(Util::Rotate(origin,pos+Vector2{roundness,dimensions.y - roundness}, rotation), roundness, color);
            DrawCircle(Util::Rotate(origin,pos+dimensions-Vector2{roundness, roundness}, rotation), roundness, color);
        }
    }

    void Renderer::DrawCircle(const Vector2& pos, float radius, const Color& color) noexcept
    {
        PushQuad(pos - Vector2{radius,radius}, {radius*2}, color, *s_circleTexture, Vertex::CIRCLE);
    }

    void Renderer::DrawBezier(const Vector2 &a, const Vector2 &cp1, const Vector2 &cp2, const Vector2 &b, float thickness,
                              const Color &color, bool rounded, float step) noexcept
    {
        Vector2 begin = a;
        for(float i = 0; i < 1.f; i += step)
        {
            const Vector2 end = Util::Cubic(a,cp1,cp2,b,i);
            DrawLine(begin, end, thickness, color, rounded);
            begin = end;
        }
    }

    void Renderer::DrawBezier(const Vector2 &a, const Vector2 &cp, const Vector2 &b, float thickness,
                              const Color &color, bool rounded, float step) noexcept
    {
        Vector2 begin = a;
        for(float i = 0; i < 1.f; i += step)
        {
            const Vector2 end = Util::Quadratic(a,cp,b,i);
            DrawLine(begin, end, thickness, color, rounded);
            begin = end;
        }
    }

    void Renderer::DrawGrid(const Vector2 &top_left, const Vector2 &size, ui32 rows, ui32 columns, const Color &color,
                            float thickness, bool rounded) noexcept
    {
        for(int i = 0; i <= rows; ++i)
        {
            DrawLine(top_left + Vector2{0.f,    (size.y / rows) * i},
                     top_left + Vector2{size.x, (size.y / rows) * i}, thickness, color, ((i == 0) || (i == rows)));
        }
        for(int i = 0; i <= columns; ++i)
            DrawLine(top_left + Vector2{(size.x / columns) * i,0.f},
                     top_left + Vector2{(size.x / columns) * i, size.y}, thickness, color, false);
    }

    void Renderer::DrawTexture(Texture &tex, const Vector2 &pos, const Vector2 &size) noexcept
    {
        PushQuad(pos, size, TRANSPARENT, tex, Vertex::TEXTURE);
    }

    void
    Renderer::PushQuad(const Vector2 &pos, const Vector2 &size, const Color &col, Texture& texture, Vertex::DrawableType type) noexcept
    {
        ui32 currentElements = s_vertexData.size();
        if(currentElements >= MAX_VERTEX_COUNT - 4)
        {
            EndBatch();
            currentElements = 0;
        }

        ui32 tex = 0;
        if(type != Vertex::RECTANGLE)
        {
            tex = PushTexture(texture);
        }
        currentElements = s_vertexData.size(); // PushTexture() might have ended the last batch, so we need to get the s_vertexData.size() again

        s_vertexData.emplace_back(Vertex{pos,                           {0.f, 0.f}, col.Hex(), tex, type});
        s_vertexData.emplace_back(Vertex{pos + Vector2{size.x, 0.f},    {1.f, 0.f}, col.Hex(), tex, type});
        s_vertexData.emplace_back(Vertex{pos + Vector2{0.f, size.y},    {0.f, 1.f}, col.Hex(), tex, type});
        s_vertexData.emplace_back(Vertex{pos + size,                    {1.f, 1.f}, col.Hex(), tex, type});

        s_indexData.emplace_back(currentElements + 0);
        s_indexData.emplace_back(currentElements + 1);
        s_indexData.emplace_back(currentElements + 2);

        s_indexData.emplace_back(currentElements + 1);
        s_indexData.emplace_back(currentElements + 3);
        s_indexData.emplace_back(currentElements + 2);
    }

    void
    Renderer::PushQuad(const Vector2 &pos, const Vector2 &size, const Color &col, Texture& texture, float rotation, Vertex::DrawableType type) noexcept
    {
        ui32 currentElements = s_vertexData.size();
        if(currentElements >= MAX_VERTEX_COUNT - 4)
        {
            EndBatch();
            currentElements = 0;
        }

        ui32 tex = 0;
        if(type != Vertex::RECTANGLE)
        {
            tex = PushTexture(texture);
        }
        currentElements = s_vertexData.size(); // PushTexture() might have ended the last batch, so we need to get the s_vertexData.size() again

        const Vector2 origin = (pos + pos + size) * 0.5f;
        s_vertexData.emplace_back(Vertex{Util::Rotate(origin, pos, rotation), {0.f, 0.f}, col.Hex(), tex, type});
        s_vertexData.emplace_back(Vertex{Util::Rotate(origin, pos + Vector2{size.x, 0.f}, rotation), {1.f, 0.f}, col.Hex(), tex, type});
        s_vertexData.emplace_back(Vertex{Util::Rotate(origin, pos + Vector2{0.f, size.y}, rotation), {0.f, 1.f}, col.Hex(), tex, type});
        s_vertexData.emplace_back(Vertex{Util::Rotate(origin, pos + size, rotation), {1.f, 1.f}, col.Hex(), tex, type});

        s_indexData.emplace_back(currentElements + 0);
        s_indexData.emplace_back(currentElements + 1);
        s_indexData.emplace_back(currentElements + 2);

        s_indexData.emplace_back(currentElements + 1);
        s_indexData.emplace_back(currentElements + 3);
        s_indexData.emplace_back(currentElements + 2);
    }

    void Renderer::DrawText(const std::string &text, const Vector2 &pos, float size, const Color &color) noexcept
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
        if(s_textures.size() >= MAX_TEXTURE_COUNT - 1)
        {
            EndBatch();
        }

        bool already_in_m_textures = false;
        const auto id = texture.GetID();
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
            texture.Bind(index);
            s_textures.push_back(id);
        }
        return index;
    }

    void Renderer::PushVertexData(std::vector<Vertex> &vertices, std::vector<ui32> &indices) noexcept
    {
        if(MAX_VERTEX_COUNT >= s_vertexData.size() + vertices.size())
            EndBatch();

        const auto size = s_indexData.size();
        std::copy(vertices.begin(), vertices.end(), back_inserter(s_vertexData));
        for(auto i : indices)
            s_indexData.push_back(size + i);
    }

    void Renderer::EndBatch() noexcept
    {
        if(s_vertexData.size() < 3)
            return;
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
        s_textures.clear();
        s_vertexBuffer->Flush();
        s_indexBuffer->Flush();
    }
}