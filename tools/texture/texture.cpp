//
// Created by felix on 17/07/2020.
//

#include "texture.h"
#include <lz4hc.h>

namespace gear {

    static size_t getFormatSize(gear::assets::PixelFormat format) {
        switch (format) {
            case gear::assets::PixelFormat_R8:
                return 1;
            case gear::assets::PixelFormat_RGB8:
                return 3;
            case gear::assets::PixelFormat_RGBA8:
                return 4;
        }
    }

    flatbuffers::Offset<gear::assets::Texture>
    buildTexture(flatbuffers::FlatBufferBuilder& fbb, int width, int height, gear::assets::PixelFormat format,
                 const uint8_t *data) {

        size_t dataSize = width * height * getFormatSize(format);

        std::vector<uint8_t> compressedData(LZ4_compressBound(dataSize));

        auto compressedSize = LZ4_compress_HC(reinterpret_cast<const char *>(data),
                                              reinterpret_cast<char *>(compressedData.data()), dataSize,
                                              compressedData.size(), LZ4HC_CLEVEL_MAX);
        compressedData.resize(compressedSize);

        return gear::assets::CreateTextureDirect(fbb, width, height, format, &compressedData);
    }

}