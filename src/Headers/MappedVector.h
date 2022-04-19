#pragma once
#include "TML/Graphics/Drawable/Vertex.h"
#include "TML/Graphics/Core/Buffers.h"

namespace tml
{
    class VertexVector : public VertexBuffer
    {
    public:
        explicit VertexVector(size_t length)
        : VertexBuffer(nullptr, sizeof(Vertex), length)
        {

        }

        void push_back(const Vertex& vertex) noexcept
        {
            if(m_mappedPtr && m_dataSize < m_capacity)
            {
                auto* dest = static_cast<Vertex*>(m_mappedPtr);
                dest[m_vertexCount] = vertex;
                ++m_vertexCount;
                m_dataSize += sizeof(Vertex);
            }
        }

        uint32_t size() const noexcept
        {
            return m_vertexCount;
        }
    };

    class IndexVector : public IndexBuffer
    {
    public:
        explicit IndexVector(size_t length)
        : IndexBuffer(nullptr, length)
        {

        }

        void push_back(uint32_t index) noexcept
        {
            if(m_mappedPtr && m_elements < m_capacity)
            {
                auto dest = ((uint32_t*) m_mappedPtr);
                dest[m_elements] = index;
                ++m_elements;
            }
        }

        uint32_t size() const noexcept
        {
            return m_elements;
        }
    };
}
