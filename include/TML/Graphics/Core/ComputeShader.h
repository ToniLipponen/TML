#pragma once
#include "Shader.h"
#include "Buffers.h"
#include <TML/System/String.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API ComputeShader : public Shader
    {
    public:
        ComputeShader();
        bool LoadFromFile(const String& filename) noexcept;
        bool LoadFromString(const String& source) noexcept;
        void ConnectBuffer(const std::string& name, uint32_t index, StorageBuffer& buffer) noexcept;
        void Dispatch(uint32_t x, uint32_t y, uint32_t z) noexcept;
        void Wait() noexcept;

    private:
        inline uint32_t GetResourceIndex(const std::string& name) noexcept;
    private:
        std::unordered_map<std::string, uint32_t> m_resourceIndexCache;
    };
}
