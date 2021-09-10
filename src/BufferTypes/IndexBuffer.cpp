#include "../internal/Buffers.h"
#include "../../external-headers//glad/glad.h"
#include "../internal/GlDebug.h"
using namespace tml;

#ifndef TML_GL_VERSION_330
IndexBuffer::IndexBuffer()
: m_id(0), m_elements(0)
{
	GL_CALL(glCreateBuffers(1, &m_id));
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) 
: m_id(0), m_elements(elements)
{
    GL_CALL(glCreateBuffers(1, &m_id));
    GL_CALL(glNamedBufferStorage(m_id, elements * 4, data, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT));
}

IndexBuffer::~IndexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::BufferData(const ui32* data, ui32 elements)
{
	m_elements = elements;
	GL_CALL(glNamedBufferStorage(m_id, elements * 4, data, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT));
}

void IndexBuffer::PushData(const ui32* data, ui32 elements)
{
    GL_CALL(glNamedBufferSubData(m_id, m_elements * sizeof(ui32), elements * sizeof(ui32), data));
	m_elements += elements;
}

void IndexBuffer::SetData(const ui32 *data, ui32 elements)
{
    GL_CALL(glNamedBufferSubData(m_id, 0, elements * sizeof(ui32), data));
	m_elements = elements;
}

void IndexBuffer::Flush()
{
	m_elements = 0;
}
#else
IndexBuffer::IndexBuffer()
: m_elements(0)
{
	GL_CALL(glad_glGenBuffers(1, &m_id));
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements)
: m_elements(elements)
{
    GL_CALL(glad_glGenBuffers(1, &m_id));
    GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui32), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GL_CALL(glad_glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Bind() const noexcept
{
    GL_CALL(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::BufferData(const ui32* data, ui32 elements)
{
	m_elements = elements;
    GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui32), data, GL_STATIC_DRAW));
}

void IndexBuffer::PushData(const ui32* data, ui32 elements)
{
    GL_CALL(glad_glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_elements*sizeof(ui32), elements*sizeof(ui32), data));
	m_elements += elements;
}

void IndexBuffer::SetData(const ui32 *data, ui32 elements)
{
    GL_CALL(glad_glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, elements*sizeof(ui32), elements*sizeof(ui32), data));
    m_elements = elements;
}

void IndexBuffer::Flush()
{
    GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW));
	m_elements = 0;
}
#endif