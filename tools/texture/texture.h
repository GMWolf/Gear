//
// Created by felix on 17/07/2020.
//

#ifndef TBTG_TEXTURE_H
#define TBTG_TEXTURE_H

#include <gear/fbs/generated/texture_generated.h>
#include <vector>

namespace gear {

    flatbuffers::Offset<gear::assets::Texture>
    buildTexture(flatbuffers::FlatBufferBuilder& builder, int width, int height, gear::assets::PixelFormat format,
                 const uint8_t *data);

}
#endif //TBTG_TEXTURE_H
