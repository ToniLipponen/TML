#pragma once
#include <TML/Audio/Common.h>
#include <TML/System/String.h>
#include <TML/Export.h>
#include <vector>

namespace tml
{
    class TML_API AudioBuffer
    {
    public:
        AudioBuffer() = default;
        explicit AudioBuffer(const String& filename);
        AudioBuffer(const void* data, size_t bytes);
        AudioBuffer(const float* samples, uint8_t channels, uint32_t rate, size_t samplesCount);

        AudioBuffer operator+(const AudioBuffer& rhs) const noexcept;
        AudioBuffer& operator+=(const AudioBuffer& rhs) noexcept;

        bool LoadFromFile(const String& filename) noexcept;                   //!< Loads sound from file.
        bool LoadFromData(const void* data, size_t bytes) noexcept;           //!< Loads sound from raw file data.
        bool LoadFromMemory(const float* data, uint8_t channels,
                            uint32_t rate, size_t samplesCount) noexcept;     //!< Loads sound from samples in memory.
        void Append(const AudioBuffer& anotherBuffer) noexcept;               //!< Appends anotherBuffer data to this buffer.
        void Clear() noexcept;                                                //!< Clears samples.
        [[nodiscard]] const std::vector<AudioFrame>& GetData() const noexcept;
        [[nodiscard]] std::vector<AudioFrame>& GetData() noexcept;
        bool WriteToFile(const String& filename) noexcept;
        friend class Sound;
    private:
        std::vector<AudioFrame> m_data;
    };
}