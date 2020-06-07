//
// Created by felix on 04/04/2020.
//

#ifndef GEAR_TILEMAP_H
#define GEAR_TILEMAP_H

#include "TileSet.h"
#include "TilemapSystem.h"


#include <memory>
#include <gear/AssetManager.h>
#include <string>

namespace gear {

    struct TileMap {

        struct Tile {
            uint32_t id : 13;
            bool hflip : 1;
            bool vflip : 1;
            bool dflip : 1;
        };

        std::string name;
        AssetReference <TileSet> tileset;
        std::unique_ptr<Tile[]> tileData;
        int width;
        int height;
        int tileWidth;
        int tileHeight;
    };

}

#endif //GEAR_TILEMAP_H
