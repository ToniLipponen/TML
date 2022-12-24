#include <TML/Audio/AudioBuffer.h>
#include <TML/System/File.h>
#include <miniaudio/miniaudio.h>
#include "Converter.h"

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
        if(auto data = File::GetBytes(filename.cpp_str()))
        {
            return LoadFromData(data->data(), data->size());
        }

        return false;
    }

    bool AudioBuffer::LoadFromData(const void *data, size_t bytes) noexcept
    {
        if(data == nullptr || bytes == 0)
        {
            return false;
        }

        static ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
        ma_decoder decoder;
        ma_result result = ma_decoder_init_memory(data, bytes, &config, &decoder);

        if(result != MA_SUCCESS)
        {
            ma_decoder_uninit(&decoder);
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

        m_data.clear();
        m_data.resize(sampleCount * 2 / channels * 48000 / rate);

        Converter converter(channels, static_cast<int>(rate), 2, 48000);
        unsigned long long framesIn = sampleCount / channels, framesOut = m_data.size();
        converter.Convert(m_data.data(), framesOut, data, framesIn);
        m_channels = 2;
        m_rate = 48000;
        return true;
    }

    void AudioBuffer::Append(const AudioBuffer& anotherBuffer) noexcept
    {
        auto& anotherData = anotherBuffer.m_data;
        m_data.insert(m_data.end(), anotherData.begin(), anotherData.end());
    }

    void AudioBuffer::Clear() noexcept
    {
        m_data.clear();
    }

	const std::vector<float>& AudioBuffer::GetData() const noexcept
	{
		return m_data;
	}

	std::vector<float>& AudioBuffer::GetData() noexcept
	{
		return m_data;
	}

    bool AudioBuffer::WriteToFile(const String &filename) noexcept
    {
        ma_encoder encoder{};
        ma_encoder_config config = ma_encoder_config_init(ma_resource_format_wav, ma_format_f32, 2, 48000);
        ma_encoder_init_file(filename.c_str(), &config, &encoder);
        ma_encoder_write_pcm_frames(&encoder, m_data.data(), m_data.size() / config.channels);
        ma_encoder_uninit(&encoder);
        return true; /// Just returning true for now.
    }
}