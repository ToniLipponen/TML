#include <TML/Audio/AudioBuffer.h>
#include <TML/System/Math.h>
#include <TML/System/File.h>
#include <miniaudio/miniaudio.h>
#include <fstream>

namespace tml
{
    AudioBuffer::AudioBuffer(const String& filename)
    {
        LoadFromFile(filename);
    }

    AudioBuffer::AudioBuffer(const void* data, size_t bytes)
    {
        LoadFromData(data, bytes);
    }

    AudioBuffer::AudioBuffer(const float* samples, uint8_t channels, uint32_t rate, size_t samplesCount)
    {
        LoadFromMemory(samples, channels, rate, samplesCount);
    }

    AudioBuffer AudioBuffer::operator+(const AudioBuffer &rhs) const noexcept
    {
        AudioBuffer newBuffer = *this;
        newBuffer.Append(rhs);
        return newBuffer;
    }

    AudioBuffer& AudioBuffer::operator+=(const AudioBuffer &rhs) noexcept
    {
        Append(rhs);
        return *this;
    }

    bool AudioBuffer::LoadFromFile(const String& filename) noexcept
    {
        std::ifstream file(filename.cpp_str(), std::ios::ate | std::ios::binary);

        size_t length = 0;
        auto data = File::GetBytes(filename.cpp_str(), length);
        return LoadFromData(data.get(), length);
    }

    bool AudioBuffer::LoadFromData(const void *data, size_t bytes) noexcept
    {
        static ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
        ma_decoder decoder;
        ma_result result = ma_decoder_init_memory(data, bytes, &config, &decoder);

        if(result != MA_SUCCESS)
        {
            return false;
        }

        size_t sampleCount = ma_decoder_get_length_in_pcm_frames(&decoder) * decoder.outputChannels;

        std::unique_ptr<float[]> samples(new float[sampleCount]);
        ma_decoder_read_pcm_frames(&decoder, samples.get(), sampleCount);
        const auto returnResult = LoadFromMemory(samples.get(), decoder.outputChannels, decoder.outputSampleRate, sampleCount);
        ma_decoder_uninit(&decoder);

        return returnResult;
    }

    bool AudioBuffer::LoadFromMemory(const float *data, uint8_t channels, uint32_t rate, size_t sampleCount) noexcept
    {
        if(data == nullptr || channels < 1 || rate == 0)
        {
            return false;
        }

        Clear();
        m_data = std::vector<float>(data, data+sampleCount);
        m_channels = channels;
        m_rate = rate;
        return true;
    }

    void AudioBuffer::Append(const AudioBuffer& anotherBuffer) noexcept
    {
        auto& anotherData = anotherBuffer.m_data;
        m_data.resize(m_data.size() + anotherBuffer.m_data.size());
        m_data.insert(m_data.end(), anotherData.begin(), anotherData.end());
    }

    void AudioBuffer::Clear() noexcept
    {
        m_data.clear();
    }
}