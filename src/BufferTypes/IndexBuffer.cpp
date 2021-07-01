#include "../../include/Buffers.h"
#include "../../include/glad/glad.h"

IndexBuffer::IndexBuffer()
: m_elements(0)
{
	glCreateBuffers(1, &m_id);
}

IndexBuffer::IndexBuffer(const ui32* data, ui32 elements) 
: m_elements(elements)
{
	glCreateBuffers(1, &m_id);
	glNamedBufferStorage(m_id, elements * 4, data, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void IndexBuffer::BufferData(const ui32* data, ui32 elements) const
{
	m_elements = elements;
	glNamedBufferStorage(m_id, elements * 4, data, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
}

void IndexBuffer::PushData(const ui32* data, ui32 elements)
{
	glNamedBufferSubData(m_id, m_elements * sizeof(ui32), elements * sizeof(ui32), data);
	m_elements += elements;
}

void IndexBuffer::SetData(const ui32 *data, ui32 elements)
{
	glNamedBufferSubData(m_id, 0, elements * sizeof(ui32), data);
	m_elements = elements;
}

void IndexBuffer::Flush()
{
	glNamedBufferStorage(m_id, 0, 0, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	m_elements = 0;
}