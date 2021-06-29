#include "../../include/Buffers.h"
#include "../../include/glad/glad.h"

VertexBuffer::VertexBuffer() 
: m_datasize(0), m_vertex_count(0)
{
	glCreateBuffers(1, &m_id);
}

VertexBuffer::VertexBuffer(const void* data, ui32 vertexsize, ui32 vertexnum) 
: m_datasize(0), m_vertex_count(0)
{
	glCreateBuffers(1, &m_id);
	glNamedBufferStorage(m_id, vertexsize * vertexnum, data, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void VertexBuffer::BufferData(void* data, ui32 vertexsize, ui32 numofvertices)
{
	m_vertex_count = numofvertices;
	m_datasize = numofvertices * vertexsize;
	glNamedBufferStorage(m_id, vertexsize * numofvertices, data, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
}

void VertexBuffer::PushData(void* data, ui32 vertexsize, ui32 numofvertices)
{
	glNamedBufferSubData(m_id, m_datasize, vertexsize * numofvertices, data);
	m_datasize += numofvertices * vertexsize;
	m_vertex_count += numofvertices;
}

void VertexBuffer::Flush()
{
	glNamedBufferData(m_id, 0, 0, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	m_datasize = 0;
	m_vertex_count = 0;
}