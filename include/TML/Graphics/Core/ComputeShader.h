#pragma once
#include "Shader.h"
#include "Buffers.h"
#include <TML/System/String.h>

namespace tml
{
    class ComputeShader : public Shader
    {
    public:
        ComputeShader();
        bool LoadFromFile(const String& filename) noexcept;
        bool LoadFromString(const String& source) noexcept;
        void ConnectBuffer(const std::string& name, uint32_t index, StorageBuffer& buffer);
        void Dispatch(uint32_t x, uint32_t y, uint32_t z);
        void Wait();
    };
}
