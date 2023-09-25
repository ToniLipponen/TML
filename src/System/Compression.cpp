#include <TML/System/Compression.h>
#include <TML/System/Math.h>
#include <fastlz/fastlz.h>

constexpr static uint64_t minimumOutputLength = 66;
constexpr static uint64_t minimumInputLength = 16;

namespace tml::Data
{
    std::vector<char> Compress(const void* data, int32_t bytes) noexcept
    {
        if(bytes < minimumInputLength || data == nullptr)
        {
            return std::vector<char>((char*)data, (char*)data + bytes);
        }

        const auto outputLength = tml::Math::Max<uint64_t>(bytes * 2, minimumOutputLength);

        std::vector<char> output(outputLength);
        const auto result = fastlz_compress(data, bytes, &output.at(0));
        output.resize(result);

        return output;
    }

    std::vector<char> Compress(const std::vector<char>& data) noexcept
    {
        return Compress(data.data(), static_cast<int32_t>(data.size()));
    }

    std::vector<char> Decompress(const void* data, int32_t bytes, int32_t maxOutput) noexcept
    {
        if(data && bytes > 0)
        {
            std::vector<char> output(maxOutput);

            /// result could be 0.
            const auto result = fastlz_decompress(data, bytes, &output[0], maxOutput);
            
            if(result)
            {
                output.resize(result);

                return output;
            }
        }

        return {};
    }

    std::vector<char> Decompress(const std::vector<char>& data, int32_t maxSize) noexcept
    {
        return Decompress(data.data(), static_cast<int32_t>(data.size()), maxSize);
    }
}