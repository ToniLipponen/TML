#include "../include/RenderWindow.h"
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include "../include/Drawable/Vertex.h"
#include "../include/glm/gtx/projection.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtx/matrix_transform_2d.hpp"

#include "../include/glm/mat4x4.hpp"
#include "../include/Utilities/Utilities.h"
#include <string>

static Texture* CircleTexture = nullptr;

static VertexArray     *m_vao = nullptr;
static VertexBuffer    *m_vertexBuffer = nullptr;
static IndexBuffer     *m_indexBuffer = nullptr;
static BufferLayout    m_layout;
static Shader          *m_shader = nullptr;

static glm::mat4        m_projection = glm::mat4(1.0f);
static glm::mat4        m_view       = glm::mat4(1.0f);

constexpr static ui32 MAX_VERTEX_COUNT  = 10000;
constexpr static ui32 MAX_TEXTURE_COUNT = 16;

static std::vector<Vertex>  m_vertexData;
static std::vector<ui32>    m_indexData;
static std::vector<ui32>    m_textures;

void Renderer::Init()
{
    m_vertexData.reserve(MAX_VERTEX_COUNT);
    m_indexData.reserve(MAX_VERTEX_COUNT*1.5);
    gladLoadGL();
    m_vao           = new VertexArray();
    m_vertexBuffer  = new VertexBuffer();
    m_indexBuffer   = new IndexBuffer();
    m_shader        = new Shader();

    m_vao->Bind();
    m_vertexBuffer->BufferData(0, sizeof(Vertex), MAX_VERTEX_COUNT);
    m_indexBuffer->BufferData(0, MAX_VERTEX_COUNT*1.5);
    m_layout.Push(2, 4);
    m_layout.Push(4, 4);
    m_layout.Push(2, 4);
    m_layout.Push(1, 4);
    m_layout.Push(1, 4);
    m_shader->Load("TML/Shaders/Vertex.vs", "TML/Shaders/Fragment.fs"); // Change these
    m_shader->Bind();
    CircleTexture = new Texture("TML/Res/Images/Circle.png");
    glad_glEnable(GL_BLEND);
    // m_shader->FromString(const std::string &vs, const std::string &fs);
}

void Renderer::Clear()
{
    glad_glClear(GL_COLOR_BUFFER_BIT);
	static int f[4];
	glad_glGetIntegerv(GL_VIEWPORT, f);
	m_projection = glm::ortho(f[0]*1.f, f[2]*1.f, f[3]*1.f, f[1]*1.f);

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
    // if(r.m_tex.HasData() && m_textures.size() >= MAX_TEXTURE_COUNT - 1)
    // {
    //     EndBatch();
    //     currentElements = 0;
    // }
    const Color c = r.m_color / 255.f;
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
            tex = 2 + m_textures.size();
            r.m_tex.Bind(tex);
            m_textures.push_back(r.m_tex.GetID());
        }
        else
        {
            tex = index+2;
            r.m_tex.Bind(tex);
        }
    }
    Vector2 origin;
    origin.x = (r.m_pos.x + r.m_pos.x + r.m_size.x) * 0.5f;
    origin.y = (r.m_pos.y + r.m_pos.y + r.m_size.y) * 0.5f;
    
    m_vertexData.push_back({Rotate(origin, r.m_pos, r.m_rotation),c, Vector2{0,1}, tex, r.m_rotation});
    m_vertexData.push_back({Rotate(origin, r.m_pos+Vector2{r.m_size.x, 0.0f}, r.m_rotation), c, Vector2{1,1}, tex, r.m_rotation});
    m_vertexData.push_back({Rotate(origin, r.m_pos+Vector2{0.0f, r.m_size.y},r.m_rotation), c, Vector2{0,0}, tex, r.m_rotation});
    m_vertexData.push_back({Rotate(origin, r.m_pos+r.m_size, r.m_rotation),                  c, Vector2{1,0}, tex, r.m_rotation});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
    std::cout << tex << std::endl;
}

void Renderer::Draw(Circle& r)
{
    ui32 currentElements = m_vertexData.size();
    if(currentElements >= MAX_VERTEX_COUNT - 4)
    {
        EndBatch();
        currentElements = 0;
    }
    
    const Color c = r.m_color / 255.f;

    CircleTexture->Bind(1);
    m_vertexData.push_back({r.m_pos,                           c, Vector2{0,1}, 1, 0});
    m_vertexData.push_back({r.m_pos+Vector2{r.m_size.x, 0.0f}, c, Vector2{1,1}, 1, 0});
    m_vertexData.push_back({r.m_pos+Vector2{0.0f, r.m_size.y}, c, Vector2{0,0}, 1, 0});
    m_vertexData.push_back({r.m_pos+r.m_size,                  c, Vector2{1,0}, 1, 0});

    m_indexData.push_back(currentElements + 0);
    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 2);

    m_indexData.push_back(currentElements + 1);
    m_indexData.push_back(currentElements + 3);
    m_indexData.push_back(currentElements + 2);
}


void Renderer::EndBatch()
{
    m_shader->UniformMat4fv("uProjection", 1, 0, &m_projection[0][0]);
    m_shader->UniformMat4fv("uView", 1, 0, &m_view[0][0]);
    for(int i = 1; i < 16; i++)
    {
        m_shader->Uniform1ui("uTextures[" + std::to_string(i) + "]", i);
    }
    
    m_vertexBuffer->SetData(m_vertexData.data(), sizeof(Vertex), m_vertexData.size());
    m_indexBuffer->SetData(m_indexData.data(), m_indexData.size());
    
    m_vao->BufferData(*m_vertexBuffer, *m_indexBuffer, m_layout);
    m_vao->Bind();
    glad_glDrawElements(GL_TRIANGLES, m_indexData.size(), GL_UNSIGNED_INT, 0);

    m_vertexData.clear();
    m_indexData.clear();
    m_textures.clear();
}
