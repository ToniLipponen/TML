#include <glad/glad.h>
#include "../internal/Buffers.h"
#include "../internal/GlDebug.h"
using namespace tml;

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