#include <TML/Audio/Capture.h>
#include <miniaudio/miniaudio.h>

namespace tml
{
    AudioCapture::AudioCapture() noexcept
    {
        ma_device_config config = ma_device_config_init(ma_device_type_capture);
        config.capture.format   = ma_format_f32;
        config.capture.channels = 1;
        config.sampleRate       = 48000;
        config.pUserData        = &m_stream;
        config.dataCallback     = [](ma_device* pDevice, [[maybe_unused]] void* pOutput, const void* pInput, ma_uint32 frameCount)
        {
            auto* stream = static_cast<AudioStream*>(pDevice->pUserData);
            static AudioBuffer buffer;

            buffer.LoadFromMemory(
                    static_cast<const float*>(pInput),
                    static_cast<uint8_t>(pDevice->capture.channels),
                    static_cast<uint32_t>(pDevice->sampleRate),
                    static_cast<size_t>(frameCount)
            );

            *stream << buffer;
        };

        m_device = new ma_device;
        auto device = static_cast<ma_device*>(m_device);
        if(ma_device_init(nullptr, &config, device) != MA_SUCCESS)
        {
            std::puts("[Error]: Failed to initialize audio input device");
        }
    }

    AudioCapture::~AudioCapture() noexcept
    {
        auto device = static_cast<ma_device*>(m_device);
        ma_device_stop(device);
        ma_device_uninit(device);
        delete device;
    }

    AudioCapture& AudioCapture::GetInstance() noexcept
    {
        static AudioCapture instance;
        return instance;
    }

    AudioStream& AudioCapture::GetStream() noexcept
    {
        return m_stream;
    }

    void AudioCapture::Start() noexcept
    {
        m_stream.Flush();
        ma_device_start(static_cast<ma_device*>(m_device));
    }

    void AudioCapture::Stop() noexcept
    {
        m_stream.Flush();
        ma_device_stop(static_cast<ma_device*>(m_device));
    }
}