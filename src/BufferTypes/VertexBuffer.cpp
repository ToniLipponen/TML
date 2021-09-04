#include "../../include/Buffers.h"
#include "../../external-headers/glad/glad.h"
#include "../../include/GlDebug.h"
using namespace tml;

#ifndef TML_GL_VERSION_330
VertexBuffer::VertexBuffer() : m_datasize(0), m_vertex_count(0)
{
    GL_CALL(glad_glCreateBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(const void* data, ui32 vertexsize, ui32 vertexnum)
: m_datasize(0), m_vertex_count(0)
{
    GL_CALL(glad_glCreateBuffers(1, &m_id));
    GL_CALL(glad_glNamedBufferStorage(m_id, vertexsize * vertexnum, data,
                                 GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT));
}

VertexBuffer::~VertexBuffer()
{
    GL_CALL(glad_glDeleteBuffers(1, &m_id));
}

void VertexBuffer::BufferData(void* data, ui32 vertexsize, ui32 numofvertices)
{
	if(data)
		m_vertex_count = numofvertices;
	m_datasize = numofvertices * vertexsize;
	GL_CALL(glad_glNamedBufferStorage(m_id, m_datasize, data,
                                 GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT));
}

void VertexBuffer::PushData(void* data, ui32 vertexsize, ui32 numofvertices)
{
    GL_CALL(glad_glNamedBufferSubData(m_id, m_datasize, vertexsize * numofvertices, data));
	m_datasize += numofvertices * vertexsize;
	m_vertex_count += numofvertices;
}

void VertexBuffer::SetData(void *data, ui32 s, ui32 n)
{
	m_datasize = s*n;
	GL_CALL(glad_glNamedBufferSubData(m_id, 0, m_datasize, data));
}

void VertexBuffer::Flush()
{
    GL_CALL(glad_glNamedBufferData(m_id, 0, 0,
                              GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT));
	m_datasize = 0;
	m_vertex_count = 0;
}
#else
VertexBuffer::VertexBuffer() : m_datasize(0), m_vertex_count(0)
{
    GL_CALL(glad_glGenBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(const void* data, ui32 vertexsize, ui32 vertexnum)
: m_datasize(0), m_vertex_count(0)
{
    GL_CALL(glad_glGenBuffers(1, &m_id));
    GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, vertexsize*vertexnum, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GL_CALL(glad_glDeleteBuffers(1, &m_id));
}

void VertexBuffer::Bind() const noexcept
{
    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::BufferData(void* data, ui32 vertexsize, ui32 numofvertices)
{
    m_vertex_count = numofvertices;
    m_datasize = numofvertices * vertexsize;
    GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, m_datasize, data, GL_STATIC_DRAW));
}

void VertexBuffer::PushData(void* data, ui32 vertexsize, ui32 numofvertices)
{
    GL_CALL(glad_glBufferSubData(GL_ARRAY_BUFFER, m_datasize, vertexsize*numofvertices, data));
    m_datasize += numofvertices * vertexsize;
    m_vertex_count += numofvertices;
}

void VertexBuffer::SetData(void *data, ui32 s, ui32 n)
{
    m_datasize = s*n;
    GL_CALL(glad_glBufferSubData(GL_ARRAY_BUFFER, 0, m_datasize, data));
}

void VertexBuffer::Flush()
{
    GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW));
    m_datasize = 0;
    m_vertex_count = 0;
}
#endif