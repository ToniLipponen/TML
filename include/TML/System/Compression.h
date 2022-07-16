#pragma once
#include <vector>
#include <cstdint>
#include <TML/Export.h>

namespace tml::File
{
    /**
     * @param data Raw data to compress.
     * @param bytes Length of the data in bytes.
     * @return Compressed data in a vector. If an error occurs, returns empty vector. */
    TML_API std::vector<uint8_t> Compress(const void* data, int32_t bytes) noexcept;


    /**
     * Function to try and compress data.
     * @param data std::vector of bytes to compress.
     * @return Compressed bytes in an std::vector. */
    TML_API std::vector<uint8_t> Compress(const std::vector<uint8_t>& data) noexcept;

    /**
     * Function to decompress compressed data.
     * @param data Raw data to decompress.
     * @param bytes Length of the data in bytes.
     * @param maxSize Upper limit for decompressed data.
     * @return Decompressed data in a vector. If an error occurs, returns empty vector. */
    TML_API std::vector<uint8_t> Decompress(const void* data, int32_t bytes, int32_t maxSize) noexcept;

    /**
     * Function to decompress compressed data.
     * @param data std::vector with compressed data.
     * @param maxSize Upper limit for decompressed data.
     * @return Decompressed bytes in an std::vector. If an error occurs, returns an empty vector. */
    TML_API std::vector<uint8_t> Decompress(const std::vector<uint8_t>& data, int32_t maxSize) noexcept;
}