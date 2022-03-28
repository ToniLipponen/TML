#pragma once
#include <vector>
#include <TML/System/String.h>

namespace tml
{
    class AudioBuffer
    {
    public:
        AudioBuffer operator+(const AudioBuffer& rhs) const noexcept;
        AudioBuffer& operator+=(const AudioBuffer& rhs) noexcept;

        bool LoadFromFile(const String& filename) noexcept;                   //!< Loads sound from file.
        bool LoadFromData(const void* data, size_t bytes) noexcept;           //!< Loads sound from raw file data.
        bool LoadFromMemory(const float* data, uint8_t channels,              //!< Loads sound from samples in memory.
                            uint32_t rate, size_t samplesCount) noexcept;
        void Append(const AudioBuffer& anotherBuffer) noexcept;               //!< Appends anotherBuffer data to this buffer.
        void Clear() noexcept;                                                //!< Clears samples.
        const std::vector<float>& GetData() const noexcept { return m_data; }

        friend class Sound;
    private:
        std::vector<float> m_data;
        uint32_t m_rate = 0;
        uint8_t m_channels = 2;
    };
}