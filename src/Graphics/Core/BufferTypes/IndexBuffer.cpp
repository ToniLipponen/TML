#include <GLHeader.h>
#include "TML/Graphics/Core/Buffers.h"
#include <GlDebug.h>
#include <cstring>

using namespace tml;


#define MAP_RANGE_FLAGS (GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT)
#define BUFFER_STORAGE_FLAGS GL_DYNAMIC_STORAGE_BIT | MAP_RANGE_FLAGS

#ifndef TML_USE_GLES
IndexBuffer::IndexBuffer() noexcept
: m_id(0), m_elements(0), m_capacity(0)
{
	GL_CALL(glCreateBuffers(1, &m_id));
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) noexcept
: m_id(0), m_elements(0), m_capacity(elements)
{
    if(data)
        m_elements = elements;

    GL_CALL(glCreateBuffers(1, &m_id));
    GL_CALL(glNamedBufferStorage(m_id, m_capacity * 4, data, BUFFER_STORAGE_FLAGS));
    m_mappedPtr = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, m_capacity, MAP_RANGE_FLAGS));
}

IndexBuffer::~IndexBuffer() noexcept
{
    GL_CALL(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Bind() const noexcept
{

}

void IndexBuffer::Unbind() const noexcept
{

}

void IndexBuffer::BufferData(const ui32* data, ui32 elements) noexcept
{
    m_capacity = elements;
    if(data)
        m_elements = elements;

    GL_CALL(glNamedBufferStorage(m_id, m_capacity * 4, data, BUFFER_STORAGE_FLAGS));
    if(m_mappedPtr)
    {
        GL_CALL(glad_glUnmapNamedBuffer(m_id));
    }
    m_mappedPtr = GL_CALL(glad_glMapNamedBufferRange(m_id, 0, m_capacity, MAP_RANGE_FLAGS));
}

void IndexBuffer::PushData(const ui32* data, ui32 elements) noexcept
{
    if(m_mappedPtr)
    {
        const ui32 size = elements * sizeof(ui32);
        auto dest = ((ui8 *) m_mappedPtr) + m_elements * sizeof(ui32);
        std::memcpy(dest, data, size);
        m_elements += elements;
    }
}

void IndexBuffer::SetData(const ui32 *data, ui32 elements) noexcept
{
    Flush();
    PushData(data, elements);
}

void IndexBuffer::Flush() noexcept
{
	m_elements = 0;
}
#else
IndexBuffer::IndexBuffer() noexcept
: m_id(0), m_elements(0), m_capacity(0)
{
    GL_CALL(glGenBuffers(1, &m_id));
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) noexcept
: m_id(0), m_elements(0), m_capacity(elements)
{
    if(data)
    {
        m_elements = m_capacity;
    }
    GL_CALL(glGenBuffers(1, &m_id));
    Bind();
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * 4, data, GL_DYNAMIC_DRAW));
    m_mappedPtr = GL_CALL(glad_glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, m_capacity, GL_MAP_WRITE_BIT));
    Unbind();
}

IndexBuffer::~IndexBuffer() noexcept
{
    GL_CALL(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Bind() const noexcept
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::Unbind() const noexcept
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::BufferData(const ui32* data, ui32 elements) noexcept
{
    m_capacity = elements;
    if(data)
        m_elements = elements;
    else
        m_elements = 0;

    Bind();
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * 4, data, GL_DYNAMIC_DRAW));
    m_mappedPtr = GL_CALL(glad_glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, m_capacity, GL_MAP_WRITE_BIT));
    Unbind();
}

void IndexBuffer::PushData(const ui32* data, ui32 elements) noexcept
{
    Bind();
    if(m_mappedPtr)
    {
        auto dest = ((ui32*) m_mappedPtr) + m_elements;
        std::memcpy(dest, data, elements*4);
        m_elements += elements;
    }
    Unbind();
}

void IndexBuffer::SetData(const ui32 *data, ui32 elements) noexcept
{
    Flush();
    PushData(data, elements);
}

void IndexBuffer::Flush() noexcept
{
    m_elements = 0;
}
#endif