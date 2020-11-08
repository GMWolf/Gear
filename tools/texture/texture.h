//
// Created by felix on 17/07/2020.
//

#ifndef TBTG_TEXTURE_H
#define TBTG_TEXTURE_H

#include <gear/texture_generated.h>
#include <vector>

namespace gear {

    flatbuffers::Offset<gear::assets::Texture>
    buildTexture(flatbuffers::FlatBufferBuilder& builder, int width, int height, gear::assets::PixelFormat format,
                 const uint8_t** data, size_t* dataSize, size_t mipCount);

}
#endif //TBTG_TEXTURE_H
