#include <TML/Graphics/Core/StorageBuffer.h>
#include "../../Headers/GLHeader.h"
#include <cstring>

#if !defined(TML_USE_GLES) && defined(TML_USE_DSA)
namespace tml
{
    StorageBuffer::StorageBuffer() noexcept
    : m_id(0)
    {
        GL_CALL(glad_glCreateBuffers(1, &m_id));
    }

    void StorageBuffer::Bind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
    }

    void StorageBuffer::Unbind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
    }

    void StorageBuffer::BufferData(const void* data, uint32_t size) const noexcept
    {
        GL_CALL(glad_glNamedBufferData(m_id, size, data, GL_DYNAMIC_COPY));
    }

    void StorageBuffer::UpdateData(const void *data, uint32_t bytes)  const noexcept
    {
        void* p = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, bytes, GL_MAP_WRITE_BIT));

        if(p && data)
            std::memcpy(p, data, bytes);

        GL_CALL(glad_glUnmapNamedBuffer(m_id));
    }

    void StorageBuffer::RetrieveData(void *data, uint32_t bytes) const noexcept
    {
        void* p = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, bytes, GL_MAP_READ_BIT));

        if(p && data)
            std::memcpy(data, p, bytes);

        GL_CALL(glad_glUnmapNamedBuffer(m_id));
    }

    void StorageBuffer::BindBufferBase(uint32_t index) const noexcept
    {
        GL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_id));
    }
}
#else
namespace tml
{
    StorageBuffer::StorageBuffer() noexcept
    : m_id(0)
    {
        GL_CALL(glad_glGenBuffers(1, &m_id));
    }

    void StorageBuffer::Bind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
    }

    void StorageBuffer::Unbind() const noexcept
    {
        GL_CALL(glad_glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
    }

    void StorageBuffer::BufferData(const void* data, uint32_t size) const noexcept
    {
        Bind();
        GL_CALL(glad_glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY));
    }

    void StorageBuffer::UpdateData(const void *data, uint32_t bytes)  const noexcept
    {
        Bind();
        void* p = GL_CALL(glad_glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, bytes, GL_MAP_WRITE_BIT));

        if(p && data)
        {
            std::memcpy(p, data, bytes);
        }

        GL_CALL(glad_glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }

    void StorageBuffer::RetrieveData(void *data, uint32_t bytes) const noexcept
    {
        Bind();
        void* p = GL_CALL(glad_glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, bytes, GL_MAP_READ_BIT));

        if(p && data)
        {
            std::memcpy(data, p, bytes);
        }

        GL_CALL(glad_glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }

    void StorageBuffer::BindBufferBase(uint32_t index) const noexcept
    {
        Bind();
        GL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_id));
    }
}
#endif