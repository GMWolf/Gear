//
// Created by felix on 04/04/2020.
//

#include <gear/map/TileSet.h>
#include <filesystem>
#include <gear/AssetManager.h>
#include <generated/tileset_generated.h>

gear::TileSet gear::loadTileSet(const assets::TileSet *tileSetDef, AssetRegistry &registry) {
    TileSet ts;
    ts.tileWidth = tileSetDef->tileWidth();
    ts.tileHeight = tileSetDef->tileHeight();
    ts.tileCount = tileSetDef->tileCount();
    ts.columnCount = tileSetDef->columns();
    ts.texture = registry.getTexture(tileSetDef->texture()->c_str());
    ts.imageWidth = tileSetDef->textureWidth();
    ts.imageHeight = tileSetDef->textureHeight();
    return ts;
}


glm::vec4 gear::TileSet::getTileUVs(int tileIndex, bool hflip, bool vflip, bool dflip) const{
    auto tileX = (tileIndex) % columnCount;
    auto tileY = (tileIndex) / columnCount;
    glm::vec4 uvs {
            (float)tileX * (float)tileWidth / (float)imageWidth,
            (float)tileY * (float)tileHeight / (float)imageHeight,
            (float)(tileX + 1) * (float)tileWidth / (float)imageWidth,
            (float)(tileY + 1) * (float)tileHeight / (float)imageHeight
    };


    if (dflip) {
        std::swap(uvs.x, uvs.y);
        std::swap(uvs.z, uvs.w);
    }
    if (hflip) {
        std::swap(uvs.x, uvs.z);
    }
    if (vflip) {
        std::swap(uvs.y, uvs.w);
    }

    return uvs;
}