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
     * @param data Raw data to decompress.
     * @param bytes Length of the data in bytes.
     * @param maxOutput Upper limit for decompressed data.
     * @return Decompressed data in a vector. If an error occurs, returns empty vector. */
    TML_API std::vector<uint8_t> Decompress(const void* data, int32_t bytes, int32_t maxOutput) noexcept;
}