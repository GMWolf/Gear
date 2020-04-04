//
// Created by felix on 04/04/2020.
//

#ifndef GEAR_TILESET_H
#define GEAR_TILESET_H

#include <string>
#include <cstdint>
#include <gear/Texture.h>
#include <gear/AssetManager.h>

namespace gear {

    struct TileSet {
        std::string name;
        AssetReference<Texture> texture;

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
    };


    struct TileSetLoader : public AssetLoader<TileSet> {
        TileSet load(const std::string& name, AssetRegistry& registry) override;
    };


}

#endif //GEAR_TILESET_H
