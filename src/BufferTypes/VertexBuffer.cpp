#include <glad/glad.h>
#include "../internal/Buffers.h"
#include "../internal/GlDebug.h"
using namespace tml;

VertexBuffer::VertexBuffer()
: m_id(0), m_datasize(0), m_vertex_count(0)
{
    GL_CALL(glad_glCreateBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(const void* data, ui32 vertexsize, ui32 vertexnum)
: m_id(0), m_datasize(0), m_vertex_count(0)
{
    GL_CALL(glad_glCreateBuffers(1, &m_id));
    GL_CALL(glad_glNamedBufferStorage(m_id, vertexsize * vertexnum, data, GL_DYNAMIC_STORAGE_BIT));
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
	GL_CALL(glad_glNamedBufferStorage(m_id, m_datasize, data, GL_DYNAMIC_STORAGE_BIT));
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
	m_datasize = 0;
	m_vertex_count = 0;
}