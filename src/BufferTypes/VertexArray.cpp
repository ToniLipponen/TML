#include "../../include/Buffers.h"
#include "../../include/glad/glad.h"
#include "../../include/GlDebug.h"

VertexArray::VertexArray() 
: m_vertex_count(0)
{
    GL_CALL(glCreateVertexArrays(1, &m_id));
}

VertexArray::VertexArray(VertexBuffer& vb, BufferLayout& layout)
: m_vertex_count(vb.VertexCount())
{
    GL_CALL(glCreateVertexArrays(1, &m_id));
	auto& lo = layout.GetData();
	ui32 offset = 0, prevsize = 0;
	for(int i = 0; i < lo.size(); ++i)
	{
	    GL_CALL(glEnableVertexArrayAttrib(m_id, i));
	    GL_CALL(glVertexArrayVertexBuffer(m_id, i, vb.m_id, prevsize, layout.GetStride()));
		prevsize = lo.at(i).second;
		GL_CALL(glVertexArrayAttribBinding(m_id, i, 0));
		GL_CALL(glVertexArrayAttribFormat(m_id, i, lo.at(i).first, GL_FLOAT, GL_FALSE, offset));
		offset += lo.at(i).first * lo.at(i).second;
	}
}

// VertexArray::~VertexArray()
// {
// 	glDeleteVertexArrays(1, &m_id);
// }

void VertexArray::BufferData(VertexBuffer& vb, BufferLayout& layout)
{
	auto& lo = layout.GetData();
	ui32 offset = 0, prevsize = 0;
	for(int i = 0; i < lo.size(); ++i)
	{
	    GL_CALL(glEnableVertexArrayAttrib(m_id, i));
	    GL_CALL(glVertexArrayVertexBuffer(m_id, i, vb.m_id, prevsize, layout.GetStride()));
		prevsize = lo.at(i).second;
		GL_CALL(glVertexArrayAttribBinding(m_id, i, 0));
		GL_CALL(glVertexArrayAttribFormat(m_id, i, lo.at(i).first, GL_FLOAT, GL_FALSE, offset));
		offset += lo.at(i).first * lo.at(i).second;
	}
	m_vertex_count = vb.VertexCount();
}

void VertexArray::BufferData(VertexBuffer& vb, IndexBuffer& ib, BufferLayout& layout)
{
	auto& lo = layout.GetData();
	ui32 offset = 0, prevsize = 0;
	for(int i = 0; i < lo.size(); ++i)
	{
	    GL_CALL(glEnableVertexArrayAttrib(m_id, i));
	    GL_CALL(glVertexArrayVertexBuffer(m_id, i, vb.m_id, prevsize, layout.GetStride()));
		prevsize = lo.at(i).second;
		GL_CALL(glVertexArrayAttribBinding(m_id, i, 0));
		GL_CALL(glVertexArrayAttribFormat(m_id, i, lo.at(i).first, GL_FLOAT, GL_FALSE, offset));
		offset += lo.at(i).first * lo.at(i).second;
	}
	GL_CALL(glVertexArrayElementBuffer(m_id, ib.m_id));
	m_vertex_count = ib.Elements();
}

void VertexArray::Bind() const
{
    GL_CALL(glBindVertexArray(m_id));
}

void VertexArray::Unbind() const
{
    GL_CALL(glBindVertexArray(0));
}
