//
// Created by felix on 04/04/2020.
//

#include <gear/map/TileMap.h>
#include <tinyxml2.h>
#include <tuple>
#include <algorithm>
#include <base64.h>

const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

gear::TileMapLoader::TileMapLoader(gear::AssetManager &assetManager) : assets(assetManager){
}


gear::AssetEntry gear::TileMapLoader::load(const std::string &fileName) {

    using namespace tinyxml2;

    XMLDocument doc;
    doc.LoadFile(fileName.c_str());


    XMLElement* xMap = doc.FirstChildElement("map");

    std::vector<std::pair<int, std::weak_ptr<const TileSet>>> tilesets;
    for(XMLElement* xTileSet = xMap->FirstChildElement("tileset"); xTileSet;
    xTileSet = xTileSet->NextSiblingElement("tileset")) {
        auto source = xTileSet->Attribute("source");
        assets.load<TileSet>(source);
        tilesets.emplace_back(
                xTileSet->IntAttribute("firstgid"),
                assets.get_as<TileSet>(source));
    }

    TileMap map = TileMap {
            {},
            xMap->IntAttribute("width"),
            xMap->IntAttribute("height"),
            xMap->IntAttribute("tileWidth"),
            xMap->IntAttribute("tileHeight")
    };


    for(XMLElement* xLayer = xMap->FirstChildElement("layer"); xLayer;
    xLayer = xLayer->NextSiblingElement("layer")) {
        TileLayer layer;
        layer.name = xLayer->Attribute("name");
        layer.width = xLayer->IntAttribute("width");
        layer.height = xLayer->IntAttribute("height");
        layer.tileset = std::find_if(tilesets.begin(), tilesets.end(), [](auto& e) {return e.first == 1;})->second;
        layer.tileData = std::make_unique<TileLayer::Tile[]>(layer.width * layer.height);
        auto data = base64_decode(xLayer->FirstChildElement("data")->GetText());
        for(int i = 0; i < data.size(); i++) {
            uint32_t tile = 0;
            tile = data[i*4+0] << 0u;
            tile = data[i*4+1] << 8u;
            tile = data[i*4+2] << 16u;
            tile = data[i*4+3] << 24u;

            layer.tileData[i].id = tile & ~(FLIPPED_DIAGONALLY_FLAG |
                                            FLIPPED_VERTICALLY_FLAG |
                                            FLIPPED_HORIZONTALLY_FLAG);
            layer.tileData[i].hflip = tile & FLIPPED_HORIZONTALLY_FLAG;
            layer.tileData[i].vflip = tile & FLIPPED_VERTICALLY_FLAG;
            layer.tileData[i].dflip = tile & FLIPPED_DIAGONALLY_FLAG;
        }
    }


}

