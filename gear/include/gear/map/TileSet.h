//
// Created by felix on 04/04/2020.
//

#ifndef GEAR_TILESET_H
#define GEAR_TILESET_H

#include <string>
#include <cstdint>
#include <gear/Texture.h>
#include <glm/vec4.hpp>
#include <gear/AssetManager.h>

namespace gear {

    struct TileSet {
        AssetReference<Texture> texture;

        TileSet() = default;
        TileSet(TileSet&&) = default;
        TileSet& operator=(TileSet&&) = default;

        int imageWidth {};
        int imageHeight {};

        int tileCount {};
        int columnCount {};
        int tileWidth {};
        int tileHeight {};
        int spacing {};
        int margin {};

        int tileOffsetX {};
        int tileOffsetY {};

        glm::vec4 getTileUVs(int tileIndex, bool hflip, bool vflip, bool dflip) const;
    };


    namespace assets {
        class TileSet;
    }
    class AssetRegistry;

    TileSet loadTileSet(const assets::TileSet* tileSetDef, AssetRegistry& registry);


}

#endif //GEAR_TILESET_H
