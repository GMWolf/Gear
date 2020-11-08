//
// Created by felix on 17/07/2020.
//

#include "texture.h"
#include <zstd.h>
#include <flatbuffers/flatbuffers.h>

namespace gear {

    static flatbuffers::Offset<gear::assets::MipData>
            buildMipData(flatbuffers::FlatBufferBuilder& fbb,const uint8_t* data, size_t dataSize) {

        std::vector<uint8_t> compressedData(ZSTD_compressBound(dataSize));
        auto compressedSize = ZSTD_compress(compressedData.data(), compressedData.size(),
                                            data, dataSize, ZSTD_maxCLevel());
        compressedData.resize(compressedSize);
        return gear::assets::CreateMipDataDirect(fbb, &compressedData);
    }

    flatbuffers::Offset<gear::assets::Texture>
    buildTexture(flatbuffers::FlatBufferBuilder& fbb, int width, int height, gear::assets::PixelFormat format,
                 const uint8_t** data, size_t* dataSize, size_t mipCount) {
       std::vector<flatbuffers::Offset<gear::assets::MipData>> mipData;
       mipData.reserve(mipCount);

        for(int i = 0; i < mipCount; i++) {
            mipData.push_back(buildMipData(fbb, data[i], dataSize[i]));
        }

        return gear::assets::CreateTextureDirect(fbb, width, height, format, &mipData);
    }

}