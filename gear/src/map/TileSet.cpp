//
// Created by felix on 04/04/2020.
//

#include <gear/map/TileSet.h>
#include <filesystem>
#include <gear/AssetManager.h>
#include <generated/tileset_generated.h>

/*
gear::TileSet gear::TileSetLoader::load(const std::string &fileName, AssetRegistry& registry) {

    using namespace tinyxml2;
    namespace fs = std::filesystem;

    auto relPath = fs::path(fileName).parent_path();

    XMLDocument doc;
    doc.LoadFile(fileName.c_str());

    XMLElement* xTileset = doc.FirstChildElement("tileset");
    XMLElement* xTileOffset = xTileset->FirstChildElement("tileoffset");
    XMLElement* xImage = xTileset->FirstChildElement("image");



    auto imageSource = xImage->Attribute("source");
    auto imagePath = relPath / imageSource;

    AssetReference texture = registry.getTexture(imagePath.string());

    TileSet ts{
        xTileset->Attribute("name"),
        texture,

        xImage->IntAttribute("width"),
        xImage->IntAttribute("height"),

        xTileset->IntAttribute("tileCount"),
        xTileset->IntAttribute("columns"),
        xTileset->IntAttribute("tilewidth"),
        xTileset->IntAttribute("tileheight"),
        xTileset->IntAttribute("spacing", 0),
        xTileset->IntAttribute("margin", 0),
        xTileOffset ? xTileOffset->IntAttribute("x", 0) : 0,
        xTileOffset ? xTileOffset->IntAttribute("y", 0) : 0
    };

    return ts;
}

*/
glm::vec4 gear::TileSet::getTileUVs(int tileIndex, bool hflip, bool vflip, bool dflip) const{
    auto tileX = (tileIndex - 1) % columnCount;
    auto tileY = (tileIndex - 1) / columnCount;
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


gear::TileSet loadTileSet(const gear::assets::TileSet* tileSetDef, gear::AssetRegistry& registry) {




}