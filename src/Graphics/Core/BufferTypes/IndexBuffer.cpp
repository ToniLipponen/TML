#include <GLHeader.h>
#include "TML/Graphics/Core/Buffers.h"
#include <GlDebug.h>
using namespace tml;

#ifndef TML_USE_GLES
IndexBuffer::IndexBuffer() noexcept
: m_id(0), m_elements(0)
{
	GL_CALL(glCreateBuffers(1, &m_id));
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) noexcept
: m_id(0), m_elements(elements)
{
    GL_CALL(glCreateBuffers(1, &m_id));
    GL_CALL(glNamedBufferStorage(m_id, elements * 4, data, GL_DYNAMIC_STORAGE_BIT));
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
	m_elements = elements;
	GL_CALL(glNamedBufferStorage(m_id, elements * 4, data, GL_DYNAMIC_STORAGE_BIT));
}

void IndexBuffer::PushData(const ui32* data, ui32 elements) noexcept
{
    GL_CALL(glNamedBufferSubData(m_id, m_elements * sizeof(ui32), elements * sizeof(ui32), data));
	m_elements += elements;
}

void IndexBuffer::SetData(const ui32 *data, ui32 elements) noexcept
{
    GL_CALL(glNamedBufferSubData(m_id, 0, elements * sizeof(ui32), data));
	m_elements = elements;
}

void IndexBuffer::Flush() noexcept
{
	m_elements = 0;
}
#else
IndexBuffer::IndexBuffer() noexcept
: m_id(0), m_elements(0)
{
    GL_CALL(glGenBuffers(1, &m_id));
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) noexcept
: m_id(0), m_elements(elements)
{
    GL_CALL(glGenBuffers(1, &m_id));
    Bind();
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * 4, data, GL_DYNAMIC_DRAW));
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
    m_elements = elements;
    Bind();
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * 4, data, GL_DYNAMIC_DRAW));
    Unbind();
}

void IndexBuffer::PushData(const ui32* data, ui32 elements) noexcept
{
    Bind();
    GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_elements * sizeof(ui32), elements * sizeof(ui32), data));
    Unbind();
    m_elements += elements;
}

void IndexBuffer::SetData(const ui32 *data, ui32 elements) noexcept
{
    Bind();
    GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, elements * sizeof(ui32), data));
    Unbind();
    m_elements = elements;
}

void IndexBuffer::Flush() noexcept
{
    m_elements = 0;
}
#endif