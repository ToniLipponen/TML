#include <TML/Graphics/Core/Buffers.h>
#include <GlDebug.h>
#include <GLHeader.h>
#include <cstring>

namespace tml
{
    StorageBuffer::StorageBuffer()
    {
        GL_CALL(glad_glGenBuffers(1, &m_id));
    }

    void StorageBuffer::Bind()
    {
        GL_CALL(glad_glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
    }

    void StorageBuffer::Unbind()
    {
        GL_CALL(glad_glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
    }

    void StorageBuffer::BufferData(const void* data, ui32 elements)
    {
        Bind();
        GL_CALL(glad_glBufferData(GL_SHADER_STORAGE_BUFFER, elements, data, GL_DYNAMIC_STORAGE_BIT));
    }

    void StorageBuffer::UpdateData(const void *data, ui32 bytes)
    {
        Bind();
        void* p = GL_CALL(glad_glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
        std::memcpy(p, data, bytes);
        GL_CALL(glad_glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }

    void StorageBuffer::RetrieveData(void *data, ui32 bytes)
    {
        Bind();
        void* p = GL_CALL(glad_glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
        std::memcpy(data, p, bytes);
        GL_CALL(glad_glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }

    void StorageBuffer::BindBufferBase(ui32 index)
    {
        GL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_id));
    }
}