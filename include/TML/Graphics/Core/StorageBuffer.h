#pragma once
#include <TML/Export.h>

#include <cstdint>

namespace tml
{
    class TML_API StorageBuffer /// Shader Storage Buffer
    {
    public:
        StorageBuffer() noexcept;
        void Bind() const noexcept;
        void Unbind() const noexcept;
        void BufferData(const void* data, uint32_t bytes) const noexcept;
        void UpdateData(const void* data, uint32_t bytes) const noexcept;
        void RetrieveData(void* data, uint32_t bytes) const noexcept;
    protected:
        void BindBufferBase(uint32_t index) const noexcept;
        friend class ComputeShader;
    private:
        uint32_t m_id;
    };
}