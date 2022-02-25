#include <TML/Graphics/Core/Buffers.h>
#include <GLHeader.h>
#include <GlDebug.h>
#include <cstring>

using namespace tml;

#ifndef TML_USE_GLES
VertexBuffer::VertexBuffer() noexcept
: m_id(0), m_dataSize(0), m_vertexCount(0), m_capacity(0)
{
    GL_CALL(glad_glCreateBuffers(1, &m_id));
}

VertexBuffer::VertexBuffer(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
: m_id(0), m_dataSize(0), m_vertexCount(0), m_capacity(vertexSize * vertexCount)
{
    if(data)
    {
        m_vertexCount = vertexCount;
        m_dataSize = m_capacity;
    }
    GL_CALL(glad_glCreateBuffers(1, &m_id));
    GL_CALL(glad_glNamedBufferStorage(m_id, m_capacity, data, GL_DYNAMIC_STORAGE_BIT));
    m_mappedPtr = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, m_capacity, GL_MAP_WRITE_BIT));
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

void VertexBuffer::BufferData(void* data, ui32 vertexSize, ui32 vertexCount) noexcept
{
    m_capacity = vertexSize * vertexCount;

    if(data)
    {
        m_vertexCount = vertexCount;
        m_dataSize = m_capacity;
    }

	GL_CALL(glad_glNamedBufferStorage(m_id, m_capacity, data, GL_DYNAMIC_STORAGE_BIT));
    m_mappedPtr = GL_CALL(glad_glMapBufferRange(GL_ARRAY_BUFFER, 0, m_capacity, GL_MAP_WRITE_BIT));
}

void VertexBuffer::SetData(void *data, ui32 s, ui32 n) noexcept
{
    m_dataSize = s * n;
	GL_CALL(glad_glNamedBufferSubData(m_id, 0, m_dataSize, data));
}

void VertexBuffer::PushData(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
{
    if(m_mappedPtr)
    {
        const ui32 size = vertexSize * vertexCount;
        auto dest = ((ui8 *) m_mappedPtr) + m_dataSize;
        std::memcpy(dest, data, size);
        m_dataSize += size;
        m_vertexCount += vertexCount;
    }
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

VertexBuffer::VertexBuffer(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
: m_id(0), m_dataSize(0), m_vertexCount(0), m_capacity(vertexSize * vertexCount)
{
    if(data)
    {
        m_vertexCount = vertexCount;
        m_dataSize = m_capacity;
    }
    GL_CALL(glGenBuffers(1, &m_id));
    Bind();
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_capacity, data, GL_DYNAMIC_DRAW));
    m_mappedPtr = GL_CALL(glad_glMapBufferRange(GL_ARRAY_BUFFER, 0, m_capacity, GL_MAP_WRITE_BIT));
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

void VertexBuffer::BufferData(void* data, ui32 vertexSize, ui32 vertexCount) noexcept
{
    m_capacity = vertexSize * vertexCount;

    if(data)
    {
        m_vertexCount = vertexCount;
        m_dataSize = m_capacity;
    }
    Bind();
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_capacity, data, GL_DYNAMIC_DRAW));
    m_mappedPtr = GL_CALL(glad_glMapBufferRange(GL_ARRAY_BUFFER, 0, m_capacity, GL_MAP_WRITE_BIT));
    Unbind();
}

void VertexBuffer::PushData(const void* data, ui32 vertexSize, ui32 vertexCount) noexcept
{
    Bind();
    if(m_mappedPtr)
    {
        const ui32 size = vertexSize * vertexCount;
        auto dest = ((ui8 *) m_mappedPtr) + m_dataSize;
        std::memcpy(dest, data, size);
        m_dataSize += size;
        m_vertexCount += vertexCount;
    }
    Unbind();
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