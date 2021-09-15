#include <glad/glad.h>
#include "../internal/Buffers.h"
#include "../internal/GlDebug.h"
using namespace tml;

VertexBuffer::VertexBuffer()
: m_id(0), m_dataSize(0), m_vertexCount(0)
{
    GL_CALL(glad_glCreateBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(const void* data, ui32 vertexsize, ui32 vertexnum)
: m_id(0), m_dataSize(0), m_vertexCount(0)
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
        m_vertexCount = numofvertices;
    m_dataSize = numofvertices * vertexsize;
	GL_CALL(glad_glNamedBufferStorage(m_id, m_dataSize, data, GL_DYNAMIC_STORAGE_BIT));
}

void VertexBuffer::PushData(void* data, ui32 vertexsize, ui32 numofvertices)
{
    GL_CALL(glad_glNamedBufferSubData(m_id, m_dataSize, vertexsize * numofvertices, data));
    m_dataSize += numofvertices * vertexsize;
    m_vertexCount += numofvertices;
}

void VertexBuffer::SetData(void *data, ui32 s, ui32 n)
{
    m_dataSize = s * n;
	GL_CALL(glad_glNamedBufferSubData(m_id, 0, m_dataSize, data));
}

void VertexBuffer::Flush()
{
    m_dataSize = 0;
    m_vertexCount = 0;
}