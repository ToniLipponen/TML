#pragma once

namespace tml
{
    VertexArray::VertexArray() noexcept
    : m_id(0), m_vertexCount(0)
    {
        GL_CALL(glGenVertexArrays(1, &m_id));
    }

    VertexArray::VertexArray(VertexBuffer& vb, BufferLayout& layout) noexcept
    : m_id(0), m_vertexCount(vb.VertexCount())
    {
        GL_CALL(glGenVertexArrays(1, &m_id));
        BufferData(vb, layout);
    }

    VertexArray::~VertexArray() noexcept
    {
        GL_CALL(glDeleteVertexArrays(1, &m_id));
    }

    void VertexArray::Bind() const noexcept
    {
        GL_CALL(glBindVertexArray(m_id));
    }

    void VertexArray::Unbind() const noexcept
    {
        GL_CALL(glBindVertexArray(0));
    }

    void VertexArray::BufferData(VertexBuffer& vb, BufferLayout& layout) noexcept
    {
        auto& lo = layout.GetData();
        uint64_t offset = 0;

        Bind();
        vb.Bind();

        for(std::size_t i = 0; i < lo.size(); i++)
        {
            GL_CALL(glEnableVertexAttribArray(i));

            if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
            {
                glVertexAttribIPointer(
                        i,
                        static_cast<int>(lo.at(i).elements),
                        lo.at(i).dataType,
                        static_cast<int>(layout.GetStride()),
                        (const void*)offset
                );
            }
            else
            {
                glVertexAttribPointer(
                        i,
                        static_cast<int>(lo.at(i).elements),
                        lo.at(i).dataType,
                        0,
                        static_cast<int>(layout.GetStride()),
                        (const void*)offset);
            }

            offset += static_cast<uint64_t>(lo.at(i).elements) * lo.at(i).size;
        }

        Unbind();
        vb.Unbind();
    }

    void VertexArray::BufferData(VertexBuffer& vb, IndexBuffer& ib, BufferLayout& layout) noexcept
    {
        auto& lo = layout.GetData();
        uint64_t offset = 0;

        Bind();
        vb.Bind();
        ib.Bind();

        for(std::size_t i = 0; i < lo.size(); i++)
        {
            GL_CALL(glEnableVertexAttribArray(i));

            if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
            {
                glVertexAttribIPointer(
                        i,
                        static_cast<int>(lo.at(i).elements),
                        lo.at(i).dataType,
                        static_cast<int>(layout.GetStride()),
                        (const void*)offset
                );
            }
            else
            {
                glVertexAttribPointer(
                        i,
                        static_cast<int>(lo.at(i).elements),
                        lo.at(i).dataType,
                        0,
                        static_cast<int>(layout.GetStride()),
                        (const void*)offset
                );
            }

            offset += static_cast<uint64_t>(lo.at(i).elements) * lo.at(i).size;
        }

        Unbind();
    }
}