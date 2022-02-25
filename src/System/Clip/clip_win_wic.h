// Clip Library
// Copyright (c) 2020 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "clip.h"

#include <algorithm>
#include <vector>

#include <shlwapi.h>
#include <wincodec.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
namespace clip {
    namespace win {

// Successful calls to CoInitialize() (S_OK or S_FALSE) must match
// the calls to CoUninitialize().
// From: https://docs.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-couninitialize#remarks
        struct coinit {
            HRESULT hr;
            coinit() {
                hr = CoInitialize(nullptr);
            }
            ~coinit() {
                if (hr == S_OK || hr == S_FALSE)
                    CoUninitialize();
            }
        };

        template<class T>
        class comptr {
        public:
            comptr() : m_ptr(nullptr) { }
            explicit comptr(T* ptr) : m_ptr(ptr) { }
            comptr(const comptr&) = delete;
            comptr& operator=(const comptr&) = delete;
            ~comptr() { reset(); }

            T** operator&() { return &m_ptr; }
            T* operator->() { return m_ptr; }
            bool operator!() const { return !m_ptr; }

            T* get() { return m_ptr; }
            void reset() {
                if (m_ptr) {
                    m_ptr->Release();
                    m_ptr = nullptr;
                }
            }
        private:
            T* m_ptr;
        };

//////////////////////////////////////////////////////////////////////
// Encode the image as PNG format

        bool write_png_on_stream(const image& image,
                                 IStream* stream) {
            const image_spec& spec = image.spec();

            comptr<IWICBitmapEncoder> encoder;
            HRESULT hr = CoCreateInstance(CLSID_WICPngEncoder,
                                          nullptr, CLSCTX_INPROC_SERVER,
                                          IID_PPV_ARGS(&encoder));
            if (FAILED(hr))
                return false;

            hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
            if (FAILED(hr))
                return false;

            comptr<IWICBitmapFrameEncode> frame;
            comptr<IPropertyBag2> options;
            hr = encoder->CreateNewFrame(&frame, &options);
            if (FAILED(hr))
                return false;

            hr = frame->Initialize(options.get());
            if (FAILED(hr))
                return false;

            // PNG encoder (and decoder) only supports GUID_WICPixelFormat32bppBGRA for 32bpp.
            // See: https://docs.microsoft.com/en-us/windows/win32/wic/-wic-codec-native-pixel-formats#png-native-codec
            WICPixelFormatGUID pixelFormat = GUID_WICPixelFormat32bppBGRA;
            hr = frame->SetPixelFormat(&pixelFormat);
            if (FAILED(hr))
                return false;

            hr = frame->SetSize(spec.width, spec.height);
            if (FAILED(hr))
                return false;

            std::vector<uint32_t> buf;
            uint8_t* ptr = (uint8_t*)image.data();
            int bytes_per_row = spec.bytes_per_row;

            // Convert to GUID_WICPixelFormat32bppBGRA if needed
            if (spec.red_mask != 0xff0000 ||
                spec.green_mask != 0xff00 ||
                spec.blue_mask != 0xff ||
                spec.alpha_mask != 0xff000000) {
                buf.resize(spec.width * spec.height);
                uint32_t* dst = (uint32_t*)&buf[0];
                uint32_t* src = (uint32_t*)image.data();
                for (int y=0; y<spec.height; ++y) {
                    auto src_line_start = src;
                    for (int x=0; x<spec.width; ++x) {
                        uint32_t c = *src;
                        *dst = ((((c & spec.red_mask  ) >> spec.red_shift  ) << 16) |
                                (((c & spec.green_mask) >> spec.green_shift) <<  8) |
                                (((c & spec.blue_mask ) >> spec.blue_shift )      ) |
                                (((c & spec.alpha_mask) >> spec.alpha_shift) << 24));
                        ++dst;
                        ++src;
                    }
                    src = (uint32_t*)(((uint8_t*)src_line_start) + spec.bytes_per_row);
                }
                ptr = (uint8_t*)&buf[0];
                bytes_per_row = 4 * spec.width;
            }

            hr = frame->WritePixels(spec.height,
                                    bytes_per_row,
                                    bytes_per_row * spec.height,
                                    (BYTE*)ptr);
            if (FAILED(hr))
                return false;

            hr = frame->Commit();
            if (FAILED(hr))
                return false;

            hr = encoder->Commit();
            if (FAILED(hr))
                return false;

            return true;
        }

        HGLOBAL write_png(const image& image) {
            coinit com;

            comptr<IStream> stream;
            HRESULT hr = CreateStreamOnHGlobal(nullptr, false, &stream);
            if (FAILED(hr))
                return nullptr;

            bool result = write_png_on_stream(image, stream.get());

            HGLOBAL handle;
            hr = GetHGlobalFromStream(stream.get(), &handle);
            if (result)
                return handle;

            GlobalFree(handle);
            return nullptr;
        }

//////////////////////////////////////////////////////////////////////
// Decode the clipboard data from PNG format

        bool read_png(const uint8_t* buf,
                      const UINT len,
                      image* output_image,
                      image_spec* output_spec) {

            int width, height, channels;
            auto* data = stbi_load_from_memory(buf, len, &width, &height, &channels, 4);
            image_spec spec;
            spec.width = width;
            spec.height = height;

            spec.bits_per_pixel = 32;
            spec.bytes_per_row = 4 * width;
            spec.red_mask    = 0xff000000;
            spec.green_mask  = 0x00ff0000;
            spec.blue_mask   = 0x0000ff00;
            spec.alpha_mask  = 0x000000ff;
            spec.red_shift   = 24;
            spec.green_shift = 16;
            spec.blue_shift  = 8;
            spec.alpha_shift = 0;

            if (output_spec)
                *output_spec = spec;

            if (output_image) {
                image img(data, spec);
                std::swap(*output_image, img);
            }
            return true;
        }

    } // namespace win
} // namespace clip
