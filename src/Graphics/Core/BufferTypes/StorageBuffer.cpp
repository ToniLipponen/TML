#include <TML/Graphics/Core/Buffers.h>
#include <GlDebug.h>
#include <GLHeader.h>
#include <cstring>

#define MAP_RANGE_FLAGS GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
#define BUFFER_STORAGE_FLAGS GL_DYNAMIC_STORAGE_BIT | MAP_RANGE_FLAGS

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
        GL_CALL(glad_glNamedBufferStorage(m_id, size, data, BUFFER_STORAGE_FLAGS));
    }

    void StorageBuffer::UpdateData(const void *data, ui32 bytes) noexcept
    {
        void* p = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, bytes, MAP_RANGE_FLAGS));
        if(p && data)
            std::memcpy(p, data, bytes);
        GL_CALL(glad_glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }

    void StorageBuffer::RetrieveData(void *data, ui32 bytes) noexcept
    {
        void* p = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, bytes, MAP_RANGE_FLAGS));
        if(data && p)
            std::memcpy(data, p, bytes);
        GL_CALL(glad_glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }

    void StorageBuffer::BindBufferBase(ui32 index) const noexcept
    {
        GL_CALL(glad_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_id));
    }
}