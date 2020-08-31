//
// Created by felix on 04/04/2020.
//

#ifndef GEAR_TILEMAP_H
#define GEAR_TILEMAP_H

#include "TileSet.h"

#include <memory>
#include <gear/AssetManager.h>
#include <string>

namespace gear {

    struct Tile {
        uint32_t id : 13;
        bool hflip : 1;
        bool vflip : 1;
        bool dflip : 1;
    };


    Tile tileDataDecode(uint32_t tile);

}

#endif //GEAR_TILEMAP_H
