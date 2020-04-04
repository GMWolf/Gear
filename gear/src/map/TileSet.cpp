//
// Created by felix on 04/04/2020.
//

#include <gear/map/TileSet.h>
#include <tinyxml2.h>
#include <filesystem>

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

    registry.load<Texture>(imagePath.string());
    AssetReference texture = registry.get<Texture>(imagePath.string());

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


