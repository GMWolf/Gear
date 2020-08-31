//
// Created by felix on 07/05/2020.
//

#include <gear/map/Map.h>

#include <generated/map_generated.h>
#include <gear/AssetManager.h>

static const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
static const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
static const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

gear::Map gear::loadMap(const assets::Map *mapDef, AssetRegistry &registry) {

    Map map;

    for(auto layerDef : *mapDef->layers()) {
        auto& layer = map.layers.emplace_back();
        layer.name = layerDef->name()->str();

        Map::TileLayer tileLayer;
        tileLayer.tileMap.tileset = registry.getTileSet(layerDef->tileSet());
        if (tileLayer.tileMap.tileset.pending()) return map;
        tileLayer.tileMap.width = layerDef->width();
        tileLayer.tileMap.height = layerDef->height();
        tileLayer.tileMap.tileWidth = tileLayer.tileMap.tileset->tileWidth;
        tileLayer.tileMap.tileHeight = tileLayer.tileMap.tileset->tileHeight;
        tileLayer.tileMap.tileData.resize(tileLayer.tileMap.width * tileLayer.tileMap.height);

        for(int i = 0; i < tileLayer.tileMap.width * tileLayer.tileMap.height; i++) {
            auto tileBlob = layerDef->data()->Get(i);
            auto& tile = tileLayer.tileMap.tileData[i];

            tile.id = tileBlob & ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
            tile.hflip = tileBlob & FLIPPED_HORIZONTALLY_FLAG;
            tile.vflip = tileBlob & FLIPPED_VERTICALLY_FLAG;
            tile.dflip = tileBlob & FLIPPED_DIAGONALLY_FLAG;
        }

        layer.variant = std::move(tileLayer);
    }

    return map;
}