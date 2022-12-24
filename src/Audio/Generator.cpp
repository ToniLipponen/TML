#include <TML/Audio/Generator.h>
#include <TML/System/Math.h>

namespace tml::AudioGenerator
{
    constexpr static double s_step = 1.0 / 48000;

    double Oscillator(double frequency, double time, SignalType type)
    {
        switch (type)
        {
            case SignalType::Sine:
            {
                return sin(time * frequency * Math::Pi * 2);
            }
            case SignalType::Square:
            {
                return (sin(time * frequency * Math::Pi * 2) < 0) ? -1.0 : 1.0;
            }

            case SignalType::Triangle:
            {
                return asin(sin(time * frequency * Math::Pi * 2)) * 2 / Math::Pi;
            }

            case SignalType::Saw:
            {
                return 0; // Todo: Implement
            }

            case SignalType::Noise:
            {
                return Math::Random(-1.0, 1.0);
            }

            default:
                break;
        }

        return 0;
    }

    AudioBuffer GenerateWaveSeconds(float length, float frequency, float amplitude, SignalType type) noexcept
    {
        return GenerateWaveSeconds(length, frequency, amplitude, type, 0);
    }

    AudioBuffer GenerateWaveSeconds(float length, float frequency, float amplitude, SignalType type, double time) noexcept
    {
        std::vector<float> samples;
        samples.reserve(static_cast<size_t>(length / s_step));

        double t = time;

        while(t <= time + length)
        {
            samples.push_back(static_cast<float>(Oscillator(frequency, t, type) * amplitude));
            t += s_step;
        }

        return {samples.data(), 1, 48000, samples.size()};
    }


    AudioBuffer GenerateWaveSamples(size_t sampleCount, float frequency, float amplitude, SignalType type) noexcept
    {
        return GenerateWaveSamples(sampleCount, frequency, amplitude, type, 0);
    }

    AudioBuffer GenerateWaveSamples(size_t sampleCount, float frequency, float amplitude, SignalType type, double time) noexcept
    {
        std::vector<float> samples;
        samples.reserve(sampleCount);

        double t = time;

        for(size_t i = 0; i < sampleCount; i++)
        {
            samples.push_back(static_cast<float>(Oscillator(frequency, t, type) * amplitude));
            t += s_step;
        }

        return {samples.data(), 1, 48000, samples.size()};
    }
}