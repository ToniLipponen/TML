#include <TML/System/Compression.h>
#include <TML/System/Math.h>
#include <fastlz/fastlz.h>

constexpr static uint64_t minimumOutputLength = 66;
constexpr static uint64_t minimumInputLength = 16;

namespace tml::File
{
    std::vector<uint8_t> Compress(const void* data, int32_t bytes) noexcept
    {
        if(bytes < minimumInputLength)
        {
            return {};
        }

        const auto outputLength = tml::Math::Max<uint64_t>(bytes * 2, minimumOutputLength);

        std::vector<uint8_t> output(outputLength);
        const auto result = fastlz_compress(data, bytes, &output.at(0));
        output.resize(result);
        return output;
    }

    std::vector<uint8_t> Compress(const std::vector<uint8_t>& data) noexcept
    {
        return Compress(data.data(), static_cast<int32_t>(data.size()));
    }

    std::vector<uint8_t> Decompress(const void* data, int32_t bytes, int32_t maxOutput) noexcept
    {
        std::vector<uint8_t> output(maxOutput);

        /// result could be 0.
        const auto result = fastlz_decompress(data, bytes, &output[0], maxOutput);
        output.resize(result);

        return output;
    }

    std::vector<uint8_t> Decompress(const std::vector<uint8_t>& data, int32_t maxSize) noexcept
    {
        return Decompress(data.data(), static_cast<int32_t>(data.size()), maxSize);
    }
}