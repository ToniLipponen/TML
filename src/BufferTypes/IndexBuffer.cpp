#include <GLHeader.h>
#include <Buffers.h>
#include <GlDebug.h>
using namespace tml;

#ifndef TML_USE_GLES
IndexBuffer::IndexBuffer()
: m_id(0), m_elements(0)
{
	GL_CALL(glCreateBuffers(1, &m_id));
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) 
: m_id(0), m_elements(elements)
{
    GL_CALL(glCreateBuffers(1, &m_id));
    GL_CALL(glNamedBufferStorage(m_id, elements * 4, data, GL_DYNAMIC_STORAGE_BIT));
}

IndexBuffer::~IndexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Bind() const
{

}

void IndexBuffer::BufferData(const ui32* data, ui32 elements)
{
	m_elements = elements;
	GL_CALL(glNamedBufferStorage(m_id, elements * 4, data, GL_DYNAMIC_STORAGE_BIT));
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
: m_id(0), m_elements(0)
{
    GL_CALL(glGenBuffers(1, &m_id));
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements)
: m_id(0), m_elements(elements)
{
    GL_CALL(glGenBuffers(1, &m_id));
    Bind();
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * 4, data, GL_DYNAMIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Bind() const
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::BufferData(const ui32* data, ui32 elements)
{
    m_elements = elements;
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * 4, data, GL_DYNAMIC_DRAW));
}

void IndexBuffer::PushData(const ui32* data, ui32 elements)
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_elements * sizeof(ui32), elements * sizeof(ui32), data));
    m_elements += elements;
}

void IndexBuffer::SetData(const ui32 *data, ui32 elements)
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, elements * sizeof(ui32), data));
    m_elements = elements;
}

void IndexBuffer::Flush()
{
    m_elements = 0;
}
#endif