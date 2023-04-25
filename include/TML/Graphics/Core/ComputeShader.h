#pragma once
#include <TML/Export.h>
#include <TML/Graphics/Core/Shader.h>
#include <TML/Graphics/Core/StorageBuffer.h>
#include <TML/System/String.h>

namespace tml
{
    class TML_API ComputeShader : public Shader
    {
    public:
        ComputeShader();
        bool LoadFromFile(const String& filename) noexcept;
        bool LoadFromString(const String& source) noexcept;
        void ConnectBuffer(uint32_t index, StorageBuffer& buffer) noexcept;
        void Dispatch(uint32_t x, uint32_t y, uint32_t z) noexcept;
        void Wait() noexcept;
    };
}
