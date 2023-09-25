#pragma once
#include <vector>
#include <cstdint>
#include <limits>
#include <TML/Export.h>

namespace tml::Data
{
    /**
     * @param data Raw data to compress.
     * @param bytes Length of the data in bytes.
     * @return Compressed data in a vector. If an error occurs, returns empty vector. */
    [[nodiscard]]
    TML_API std::vector<char> Compress(const void* data, int32_t bytes) noexcept;

    /**
     * Function to try and compress data.
     * @param data std::vector of bytes to compress.
     * @return Compressed bytes in an std::vector. */
    [[nodiscard]]
    TML_API std::vector<char> Compress(const std::vector<char>& data) noexcept;

    /**
     * Function to decompress compressed data.
     * @param data Raw data to decompress.
     * @param bytes Length of the data in bytes.
     * @param maxSize Upper limit for decompressed data.
     * @return Decompressed data in a vector. If an error occurs, returns empty vector. */
    [[nodiscard]]
    TML_API std::vector<char> Decompress(const void* data, int32_t bytes, int32_t maxSize = std::numeric_limits<int32_t>::max()) noexcept;

    /**
     * Function to decompress compressed data.
     * @param data std::vector with compressed data.
     * @param maxSize Upper limit for decompressed data.
     * @return Decompressed bytes in an std::vector. If an error occurs, returns an empty vector. */
    [[nodiscard]]
    TML_API std::vector<char> Decompress(const std::vector<char>& data, int32_t maxSize = std::numeric_limits<int32_t>::max()) noexcept;
}