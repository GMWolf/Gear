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
