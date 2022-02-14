#pragma once
#include "Shader.h"
#include "Buffers.h"
#include <TML/System/String.h>

namespace tml
{
    class ComputeShader : public Shader
    {
    public:
        ComputeShader() = default;
        bool LoadFromFile(const String& filename) noexcept;
        bool LoadFromString(const String& source) noexcept;
        void ConnectBuffer(const String& name, ui32 index, StorageBuffer& buffer);
        void Dispatch(i16 x, i16 y);
        void Wait();
    };
}
