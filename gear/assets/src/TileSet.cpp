//
// Created by felix on 04/04/2020.
//

#include <TileSet.h>
#include <gear/tileset_generated.h>

glm::vec4 gear::getTileUVs(const assets::TileSet* tileSet, int tileIndex, bool hflip, bool vflip, bool dflip) {
    auto tileX = (tileIndex) % tileSet->columns();
    auto tileY = (tileIndex) / tileSet->columns();
    glm::vec4 uvs {
            (float)tileX * (float)tileSet->tileWidth() / (float)tileSet->textureWidth(),
            (float)tileY * (float)tileSet->tileHeight() / (float)tileSet->textureHeight(),
            (float)(tileX + 1) * (float)tileSet->tileWidth() / (float)tileSet->textureWidth(),
            (float)(tileY + 1) * (float)tileSet->tileHeight() / (float)tileSet->textureHeight()
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