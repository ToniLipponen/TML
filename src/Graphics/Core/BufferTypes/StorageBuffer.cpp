#include <TML/Graphics/Core/Buffers.h>
#include <GlDebug.h>
#include <GLHeader.h>
#include <cstring>

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

    void StorageBuffer::BufferData(const void* data, ui32 size) noexcept
    {
        Bind();
        GL_CALL(glad_glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY));
    }

    void StorageBuffer::UpdateData(const void *data, ui32 bytes) noexcept
    {
        Bind();
        void* p = GL_CALL(glad_glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
        if(p)
            std::memcpy(p, data, bytes);
        GL_CALL(glad_glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }

    void StorageBuffer::RetrieveData(void *data, ui32 bytes) noexcept
    {
        Bind();
        void* p = GL_CALL(glad_glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
        if(data && p)
            std::memcpy(data, p, bytes);
        GL_CALL(glad_glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }

    void StorageBuffer::BindBufferBase(ui32 index) const noexcept
    {
        GL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_id));
    }
}