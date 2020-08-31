//
// Created by felix on 04/04/2020.
//

#ifndef GEAR_TILESET_H
#define GEAR_TILESET_H

#include <string>
#include <cstdint>
#include <glm/vec4.hpp>
#include <gear/Assets.h>

namespace gear {

    namespace assets {
        class TileSet;
    }

    glm::vec4 getTileUVs(const assets::TileSet* tileSet, int tileIndex, bool hflip, bool vflip, bool dflip);
}

#endif //GEAR_TILESET_H
