#pragma once
#include <TML/Graphics/Drawable/Vertex.h>
#include <TML/Graphics/Core/Buffers.h>

/// \brief These are just vectors that map to GPU memory.
/// This is to reduce the number of copies required.

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
            if(m_vertexCount*sizeof(Vertex) < m_capacity)
            {
                auto dest = ((Vertex*) m_mappedPtr);
                dest[m_vertexCount] = vertex;
                ++m_vertexCount;
                m_dataSize += sizeof(Vertex);
            }
        }

        ui64 size() const noexcept
        {
            return m_vertexCount;
        }

        void clear() noexcept
        {
            Flush();
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
            if(m_elements < m_capacity)
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

        void clear() noexcept
        {
            Flush();
        }
    };
}
