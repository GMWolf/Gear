//
// Created by felix on 04/04/2020.
//

#include <gear/map/TileSet.h>
#include <tinyxml2.h>

gear::AssetEntry gear::TileSetLoader::load(const std::string &fileName) {

    using namespace tinyxml2;

    XMLDocument doc;
    doc.LoadFile(fileName.c_str());

    XMLElement* xTileset = doc.FirstChildElement("tileset");
    XMLElement* xTileOffset = xTileset->FirstChildElement("tileoffset");
    XMLElement* xImage = xTileset->FirstChildElement("image");

    return {std::make_shared<TileSet>(TileSet{
        xImage->Attribute("name"),
        Texture(xImage->Attribute("source")),

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


