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

    struct TileLayer {
        std::string name;
        std::weak_ptr<const TileSet> tileset;
        std::unique_ptr<uint16_t[]> tileData;
        int width;
        int height;
    };

    struct TileMap {
        std::vector<TileLayer> layers;
        int width;
        int height;
        int tileWidth;
        int tileHeight;
    };


    struct TileMapLoader : public AssetLoader {
        AssetManager& assets;
        explicit TileMapLoader(AssetManager& assetManager);
        AssetEntry load(const std::string& name) override;
    };

}

#endif //GEAR_TILEMAP_H
