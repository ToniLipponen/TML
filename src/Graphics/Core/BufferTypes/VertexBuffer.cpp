#include <TML/Graphics/Core/Buffers.h>
#include <GLHeader.h>
#include <GlDebug.h>
using namespace tml;

#ifndef TML_USE_GLES
VertexBuffer::VertexBuffer() noexcept
: m_id(0), m_dataSize(0), m_vertexCount(0)
{
    GL_CALL(glad_glCreateBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(const void* data, ui32 vertexsize, ui32 vertexnum) noexcept
: m_id(0), m_dataSize(0), m_vertexCount(0)
{
    GL_CALL(glad_glCreateBuffers(1, &m_id));
    GL_CALL(glad_glNamedBufferStorage(m_id, vertexsize * vertexnum, data, GL_DYNAMIC_STORAGE_BIT));
}

VertexBuffer::~VertexBuffer() noexcept
{
    GL_CALL(glad_glDeleteBuffers(1, &m_id));
}

void VertexBuffer::Bind() const noexcept
{

}

void VertexBuffer::Unbind() const noexcept
{

}

void VertexBuffer::BufferData(void* data, ui32 vertexsize, ui32 numofvertices) noexcept
{
	if(data)
        m_vertexCount = numofvertices;
    m_dataSize = numofvertices * vertexsize;
	GL_CALL(glad_glNamedBufferStorage(m_id, m_dataSize, data, GL_DYNAMIC_STORAGE_BIT));
}

void VertexBuffer::PushData(void* data, ui32 vertexsize, ui32 numofvertices) noexcept
{
    const ui32 size = vertexsize * numofvertices;
    GL_CALL(glad_glNamedBufferSubData(m_id, m_dataSize, size, data));
    m_dataSize += size;
    m_vertexCount += numofvertices;
}

void VertexBuffer::SetData(void *data, ui32 s, ui32 n) noexcept
{
    m_dataSize = s * n;
	GL_CALL(glad_glNamedBufferSubData(m_id, 0, m_dataSize, data));
}

void VertexBuffer::Flush() noexcept
{
    m_dataSize = 0;
    m_vertexCount = 0;
}
#else

VertexBuffer::VertexBuffer() noexcept
: m_id(0), m_dataSize(0), m_vertexCount(0)
{
    GL_CALL(glGenBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(const void* data, ui32 vertexsize, ui32 vertexnum) noexcept
: m_id(0), m_dataSize(0), m_vertexCount(0)
{
    GL_CALL(glGenBuffers(1, &m_id));
    Bind();
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertexsize * vertexnum, data, GL_DYNAMIC_DRAW));
    Unbind();
}

VertexBuffer::~VertexBuffer() noexcept
{
    GL_CALL(glad_glDeleteBuffers(1, &m_id));
}

void VertexBuffer::Bind() const noexcept
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::Unbind() const noexcept
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::BufferData(void* data, ui32 vertexsize, ui32 numofvertices) noexcept
{
    if(data)
        m_vertexCount = numofvertices;
    m_dataSize = numofvertices * vertexsize;
    Bind();
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_dataSize, data, GL_DYNAMIC_DRAW));
    Unbind();
}

void VertexBuffer::PushData(void* data, ui32 vertexsize, ui32 numofvertices) noexcept
{
    Bind();
    GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, m_dataSize, vertexsize * numofvertices, data));
    Unbind();
    m_dataSize += numofvertices * vertexsize;
    m_vertexCount += numofvertices;
}

void VertexBuffer::SetData(void *data, ui32 s, ui32 n) noexcept
{
    m_dataSize = s * n;
    Bind();
    GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, m_dataSize, data));
    Unbind();
}

void VertexBuffer::Flush() noexcept
{
    m_dataSize = 0;
    m_vertexCount = 0;
}
#endif