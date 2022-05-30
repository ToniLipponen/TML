#pragma once
#include <cstdint>

namespace tml
{
    VertexArray::VertexArray() noexcept
    : m_id(0), m_vertexCount(0)
    {
        GL_CALL(glad_glCreateVertexArrays(1, &m_id));
    }

    VertexArray::VertexArray(VertexBuffer& vb, BufferLayout& layout) noexcept
    : m_id(0), m_vertexCount(vb.VertexCount())
    {
        GL_CALL(glad_glCreateVertexArrays(1, &m_id));
        auto& lo = layout.GetData();
        uint32_t offset = 0;
        GL_CALL(glad_glVertexArrayVertexBuffer(m_id, 0, vb.m_id, 0, layout.GetStride()));

        for(uint64_t i = 0; i < lo.size(); i++)
        {
            GL_CALL(glad_glEnableVertexArrayAttrib(m_id, i));

            if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
            {
                GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, lo.at(i).elements, lo.at(i).dataType, offset));
            }
            else
            {
                GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, lo.at(i).elements, GL_FLOAT, GL_FALSE, offset));
            }

            GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
            offset += lo.at(i).elements * lo.at(i).size;
        }
    }

    VertexArray::~VertexArray() noexcept
    {
        GL_CALL(glad_glDeleteVertexArrays(1, &m_id));
    }

    void VertexArray::Bind() const noexcept
    {
        GL_CALL(glad_glBindVertexArray(m_id));
    }

    void VertexArray::Unbind() const noexcept
    {
        GL_CALL(glad_glBindVertexArray(0));
    }

    void VertexArray::BufferData(VertexBuffer& vb, BufferLayout& layout) noexcept
    {
        auto& lo = layout.GetData();
        uint32_t offset = 0;
        GL_CALL(glad_glVertexArrayVertexBuffer(m_id, 0, vb.m_id, 0, layout.GetStride()));
        for(uint64_t i = 0; i < lo.size(); i++)
        {
            GL_CALL(glad_glEnableVertexArrayAttrib(m_id, i));

            if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
            {
                GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, lo.at(i).elements, lo.at(i).dataType, offset));
            }
            else
            {
                GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, lo.at(i).dataType, GL_FLOAT, GL_FALSE, offset));
            }

            GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
            offset += lo.at(i).elements * lo.at(i).size;
        }
        m_vertexCount = vb.VertexCount();
    }

    void VertexArray::BufferData(VertexBuffer& vb, IndexBuffer& ib, BufferLayout& layout) noexcept
    {
        auto& lo = layout.GetData();
        uint32_t offset = 0;
        GL_CALL(glad_glVertexArrayVertexBuffer(m_id, 0, vb.m_id, 0, layout.GetStride()));

        for(uint64_t i = 0; i < lo.size(); i++)
        {
            GL_CALL(glad_glEnableVertexArrayAttrib(m_id, i));

            if(lo.at(i).dataType != BufferLayout::VERTEX_FLOAT)
            {
                GL_CALL(glad_glVertexArrayAttribIFormat(m_id, i, lo.at(i).elements, lo.at(i).dataType, offset));
            }
            else
            {
                GL_CALL(glad_glVertexArrayAttribFormat(m_id, i, lo.at(i).elements, GL_FLOAT, GL_FALSE, offset));
            }

            GL_CALL(glad_glVertexArrayAttribBinding(m_id, i, 0));
            offset += lo.at(i).elements * lo.at(i).size;
        }

        GL_CALL(glad_glVertexArrayElementBuffer(m_id, ib.m_id));
        m_vertexCount = ib.Elements();
    }
}