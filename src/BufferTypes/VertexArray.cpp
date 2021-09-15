#include <glad/glad.h>
#include "../internal/Buffers.h"
#include "../internal/GlDebug.h"
using namespace tml;

VertexArray::VertexArray()
: m_id(0), m_vertexCount(0)
{
    GL_CALL(glCreateVertexArrays(1, &m_id));
}

VertexArray::VertexArray(VertexBuffer& vb, BufferLayout& layout)
: m_id(0), m_vertexCount(vb.VertexCount())
{
    GL_CALL(glCreateVertexArrays(1, &m_id));
    auto& lo = layout.GetData();
    ui32 offset = 0;
    GL_CALL(glad_glVertexArrayVertexBuffer(m_id, 0, vb.m_id, 0, layout.GetStride()));
    for(int i = 0; i < lo.size(); i++)
    {
        GL_CALL(glad_glEnableVertexArrayAttrib(m_id, i));
        if(std::get<2>(lo.at(i)) != BufferLayout::VERTEX_FLOAT)
        {
            GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, std::get<0>(lo.at(i)), std::get<2>(lo.at(i)), offset));
        }
        else
            GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, std::get<0>(lo.at(i)), GL_FLOAT, GL_FALSE, offset));
        GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
        offset += std::get<0>(lo.at(i)) * std::get<1>(lo.at(i));
    }
}

 VertexArray::~VertexArray()
 {
 	GL_CALL(glDeleteVertexArrays(1, &m_id));
 }

void VertexArray::BufferData(VertexBuffer& vb, BufferLayout& layout) noexcept
{
    auto& lo = layout.GetData();
    ui32 offset = 0;
    GL_CALL(glad_glVertexArrayVertexBuffer(m_id, 0, vb.m_id, 0, layout.GetStride()));
    for(int i = 0; i < lo.size(); i++)
    {
        GL_CALL(glad_glEnableVertexArrayAttrib(m_id, i));
        if(std::get<2>(lo.at(i)) != BufferLayout::VERTEX_FLOAT)
        {
            GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, std::get<0>(lo.at(i)), std::get<2>(lo.at(i)), offset));
        }
        else
            GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, std::get<0>(lo.at(i)), GL_FLOAT, GL_FALSE, offset));
        GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
        offset += std::get<0>(lo.at(i)) * std::get<1>(lo.at(i));
    }
    m_vertexCount = vb.VertexCount();
}

void VertexArray::BufferData(VertexBuffer& vb, IndexBuffer& ib, BufferLayout& layout) noexcept
{
	auto& lo = layout.GetData();
	ui32 offset = 0;
    GL_CALL(glad_glVertexArrayVertexBuffer(m_id, 0, vb.m_id, 0, layout.GetStride()));
	for(int i = 0; i < lo.size(); i++)
	{
	    GL_CALL(glad_glEnableVertexArrayAttrib(m_id, i));
        if(std::get<2>(lo.at(i)) != BufferLayout::VERTEX_FLOAT)
        {
            GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, std::get<0>(lo.at(i)), std::get<2>(lo.at(i)), offset));
        }
        else
            GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, std::get<0>(lo.at(i)), GL_FLOAT, GL_FALSE, offset));
		GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
        offset += std::get<0>(lo.at(i)) * std::get<1>(lo.at(i));
	}
	GL_CALL(glVertexArrayElementBuffer(m_id, ib.m_id));
    m_vertexCount = ib.Elements();
}

void VertexArray::Bind() const noexcept
{
    GL_CALL(glBindVertexArray(m_id));
}

void VertexArray::Unbind() const noexcept
{
    GL_CALL(glBindVertexArray(0));
}