//
// Created by felix on 04/04/2020.
//

#include <gear/map/TileMap.h>
#include <tuple>
#include <algorithm>
#include <filesystem>

const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

gear::Tile gear::tileDataDecode(uint32_t tileBlob) {
    Tile tile{};
    tile.id = tileBlob & ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
    tile.hflip = tileBlob & FLIPPED_HORIZONTALLY_FLAG;
    tile.vflip = tileBlob & FLIPPED_VERTICALLY_FLAG;
    tile.dflip = tileBlob & FLIPPED_DIAGONALLY_FLAG;
    return tile;
}
