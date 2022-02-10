#pragma once
#include "Shader.h"
#include "TML/System/String.h"

namespace tml
{
    class ComputeShader : public Shader
    {
    public:
        ComputeShader() = default;
        void LoadFromFile(const String& filename) noexcept;
        void LoadFromString(const String& source) noexcept;
        void ConnectBuffer(const String& name, ui32 index);
        void Dispatch(i16 x, i16 y);
        void Wait();
    };
}
