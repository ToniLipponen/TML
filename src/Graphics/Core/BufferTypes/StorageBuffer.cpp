#include <TML/Graphics/Core/Buffers.h>
#include <GlDebug.h>
#include <GLHeader.h>
#include <cstring>

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

    void StorageBuffer::BufferData(const void* data, ui32 size) noexcept
    {
        Bind();
        GL_CALL(glad_glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_STORAGE_BIT));
    }

    void StorageBuffer::UpdateData(const void *data, ui32 bytes) noexcept
    {
        void* p = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, bytes, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        if(p && data)
            std::memcpy(p, data, bytes);

        GL_CALL(glad_glUnmapNamedBuffer(m_id));
    }

    void StorageBuffer::RetrieveData(void *data, ui32 bytes) noexcept
    {
        void* p = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, bytes, GL_MAP_READ_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        if(data && p)
            std::memcpy(data, p, bytes);

        GL_CALL(glad_glUnmapNamedBuffer(m_id));
    }

    void StorageBuffer::BindBufferBase(ui32 index) const noexcept
    {
        GL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_id));
    }
}