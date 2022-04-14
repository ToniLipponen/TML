#pragma once
#include "TML/Graphics/Drawable/Vertex.h"
#include "TML/Graphics/Core/Buffers.h"

/// \brief These are just vectors that map to GPU memory.

namespace tml
{
    class VertexVector : public VertexBuffer
    {
    public:
        explicit VertexVector(ui64 length)
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

        ui64 size() const noexcept
        {
            return m_vertexCount;
        }
    };

    class IndexVector : public IndexBuffer
    {
    public:
        explicit IndexVector(ui64 length)
        : IndexBuffer(nullptr, length)
        {

        }

        void push_back(ui32 index) noexcept
        {
            if(m_mappedPtr && m_elements < m_capacity)
            {
                auto dest = ((ui32*) m_mappedPtr);
                dest[m_elements] = index;
                ++m_elements;
            }
        }

        ui64 size() const noexcept
        {
            return m_elements;
        }
    };
}
