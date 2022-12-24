#pragma once
#include <TML/Audio/AudioBuffer.h>

namespace tml::AudioGenerator
{
    enum class SignalType { Sine, Square, Triangle, Saw, Noise };

    TML_API AudioBuffer GenerateWaveSeconds(float length, float frequency, float amplitude, SignalType type) noexcept;
    TML_API AudioBuffer GenerateWaveSeconds(float length, float frequency, float amplitude, SignalType type, double time) noexcept;
    TML_API AudioBuffer GenerateWaveSamples(size_t sampleCount, float frequency, float amplitude, SignalType type) noexcept;
    TML_API AudioBuffer GenerateWaveSamples(size_t sampleCount, float frequency, float amplitude, SignalType type, double time) noexcept;
}