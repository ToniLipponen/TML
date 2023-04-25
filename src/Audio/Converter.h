
#pragma once
#include <miniaudio/miniaudio.h>

class Converter
{
public:
    Converter(int inputChannels, int inputRate, int outputChannels, int outputRate) noexcept
    {
        m_config = ma_data_converter_config_init(
                ma_format_f32,
                ma_format_f32,
                inputChannels,
                outputChannels,
                inputRate,
                outputRate
        );

        ma_data_converter_init(&m_config, &m_converter);
    }

    ~Converter()
    {
        ma_data_converter_uninit(&m_converter);
    }

    void Convert(float* destination, unsigned long long& framesOut, const float* source, unsigned long long& framesIn) noexcept
    {
        ma_data_converter_process_pcm_frames(&m_converter, source, &framesIn, destination, &framesOut);
    }

private:
    ma_data_converter m_converter{};
    ma_data_converter_config m_config{};
};