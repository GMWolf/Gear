//
// Created by felix on 04/04/2020.
//

#include <gear/map/TileSet.h>
#include <tinyxml2.h>
#include <filesystem>

gear::AssetEntry gear::TileSetLoader::load(const std::string &fileName) {

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

    return {std::make_shared<TileSet>(TileSet{
        xTileset->Attribute("name"),
        Texture(imagePath.string()),

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
    })};
}


