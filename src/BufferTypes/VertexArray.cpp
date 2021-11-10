#include <GLHeader.h>
#include <Buffers.h>
#include <GlDebug.h>
using namespace tml;

#ifndef TML_USE_GLES
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
        if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
        {
            GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, lo.at(i).elements, lo.at(i).dataType, offset));
        }
        else
            GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, lo.at(i).elements, GL_FLOAT, GL_FALSE, offset));
        GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
        offset += lo.at(i).elements * lo.at(i).size;
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
        if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
        {
            GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, lo.at(i).elements, lo.at(i).dataType, offset));
        }
        else
            GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, lo.at(i).dataType, GL_FLOAT, GL_FALSE, offset));
        GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
        offset += lo.at(i).elements * lo.at(i).size;
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
        if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
        {
            GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, lo.at(i).elements, lo.at(i).dataType, offset));
        }
        else
            GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, lo.at(i).elements, GL_FLOAT, GL_FALSE, offset));
		GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
        offset += lo.at(i).elements * lo.at(i).size;
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
#else
VertexArray::VertexArray()
: m_id(0), m_vertexCount(0)
{
    GL_CALL(glGenVertexArrays(1, &m_id));
}

VertexArray::VertexArray(VertexBuffer& vb, BufferLayout& layout)
: m_id(0), m_vertexCount(vb.VertexCount())
{
    GL_CALL(glGenVertexArrays(1, &m_id));

    auto& lo = layout.GetData();
    ui64 offset = 0;

    BufferData(vb, layout);
}

VertexArray::~VertexArray()
{
    GL_CALL(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::Bind() const noexcept
{
    GL_CALL(glBindVertexArray(m_id));
}

void VertexArray::Unbind() const noexcept
{
    GL_CALL(glBindVertexArray(0));
}

void VertexArray::BufferData(VertexBuffer& vb, BufferLayout& layout) noexcept
{
    auto& lo = layout.GetData();
    ui64 offset = 0;

    Bind();
    vb.Bind();
    for(int i = 0; i < lo.size(); i++)
    {
        GL_CALL(glEnableVertexAttribArray(i));
        if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
            glVertexAttribIPointer(i, lo.at(i).elements, lo.at(i).dataType, layout.GetStride(), (const void*)offset);
        else
            glVertexAttribPointer(i, lo.at(i).elements, lo.at(i).dataType, 0, layout.GetStride(), (const void*)offset);
        offset += lo.at(i).elements * lo.at(i).size;
    }
    Unbind();
    vb.Unbind();
}

void VertexArray::BufferData(VertexBuffer& vb, IndexBuffer& ib, BufferLayout& layout) noexcept
{
    auto& lo = layout.GetData();
    ui64 offset = 0;

    Bind();
    vb.Bind();
    ib.Bind();
    for(int i = 0; i < lo.size(); i++)
    {
        GL_CALL(glEnableVertexAttribArray(i));
        if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
            glVertexAttribIPointer(i, lo.at(i).elements, lo.at(i).dataType, layout.GetStride(), (const void*)offset);
        else
            glVertexAttribPointer(i, lo.at(i).elements, lo.at(i).dataType, 0, layout.GetStride(), (const void*)offset);
        offset += lo.at(i).elements * lo.at(i).size;
    }
    Unbind();
}

#endif