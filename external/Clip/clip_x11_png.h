// Clip Library
// Copyright (c) 2018-2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

/**
 * @brief Hello. I didn't want to to link against libPNG, so I changed this to use stbi instead for png read/write. - Toni Lipponen
 */

#include "clip.h"

#include <algorithm>
#include <vector>

#include "stb_image.h"
#include "stb_image_write.h"

namespace clip {
namespace x11 {

void write_data_func(void* context, void* data, int size)
{
    auto& output = *((std::vector<uint8_t>*)context);
    output.reserve(size);
    std::memcpy(&output[0], data, size);
}

bool write_png(const image& image, std::vector<uint8_t>& output)
{
    const auto w = image.spec().width;
    const auto h = image.spec().height;
    const auto Bpp = image.spec().bits_per_pixel / 8;
    return stbi_write_png_to_func(write_data_func, &output, w, h, Bpp, image.data(), w*Bpp);
}

bool read_png(const uint8_t* buf,
              const size_t len,
              image* output_image,
              image_spec* output_spec)
{

  int width, height;
  int bit_depth, color_type, interlace_type;
  auto* data = stbi_load_from_memory(buf, len, &width, &height, &bit_depth, 4);

  image_spec spec;
  spec.width = width;
  spec.height = height;
  spec.bits_per_pixel = 32;
  spec.bytes_per_row = 4*width;

  spec.red_mask    = 0x000000ff;
  spec.green_mask  = 0x0000ff00;
  spec.blue_mask   = 0x00ff0000;
  spec.alpha_mask  = 0xff000000;

  spec.red_shift   = 0;
  spec.green_shift = 8;
  spec.blue_shift  = 16;
  spec.alpha_shift = 24;

  if (output_spec)
    *output_spec = spec;

  if (output_image &&
      width > 0 &&
      height > 0) {
    image img(data, spec);
    std::swap(*output_image, img);
  }
  return true;
}

} // namespace x11
} // namespace clip
