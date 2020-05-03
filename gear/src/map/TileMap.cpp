//
// Created by felix on 04/04/2020.
//

#include <gear/map/TileMap.h>
#include <tinyxml2.h>
#include <tuple>
#include <algorithm>
#include <base64.h>
#include <filesystem>

const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

gear::TileMap gear::TileMapLoader::load(const std::string &fileName, AssetRegistry& registry) {

    using namespace tinyxml2;
    namespace fs = std::filesystem;

    auto relPath = fs::path(fileName).parent_path();

    XMLDocument doc;
    doc.LoadFile(fileName.c_str());


    XMLElement* xMap = doc.FirstChildElement("map");

    std::vector<std::pair<int, AssetReference <TileSet>>> tilesets;

    for(XMLElement* xTileSet = xMap->FirstChildElement("tileset"); xTileSet;
    xTileSet = xTileSet->NextSiblingElement("tileset")) {
        auto source = xTileSet->Attribute("source");
        auto path = relPath / source;
        /*registry.load<TileSet>(path.string());
        tilesets.emplace_back(
                xTileSet->IntAttribute("firstgid"),
                registry.get<TileSet>(path.string()));*/
    }

    TileMap map = TileMap {
            {},
            xMap->IntAttribute("width"),
            xMap->IntAttribute("height"),
            xMap->IntAttribute("tilewidth"),
            xMap->IntAttribute("tileheight")
    };


    for(XMLElement* xLayer = xMap->FirstChildElement("layer"); xLayer;
    xLayer = xLayer->NextSiblingElement("layer")) {
        TileLayer layer;
        layer.name = xLayer->Attribute("name");
        layer.width = xLayer->IntAttribute("width");
        layer.height = xLayer->IntAttribute("height");
        layer.tileset = std::find_if(tilesets.begin(), tilesets.end(), [](auto& e) {return e.first == 1;})->second;
        layer.tileData = std::make_unique<TileLayer::Tile[]>(layer.width * layer.height);
        std::string database64 = std::string(xLayer->FirstChildElement("data")->GetText());
        database64.erase(database64.begin(), std::find_if(database64.begin(), database64.end(), [](char c) {return !std::isspace(c);}));
        auto data = base64_decode(database64);
        for(int i = 0; i < (layer.width * layer.height); i++) {
            uint32_t tile = 0;
            tile |= data[i*4+0] << 0u;
            tile |= data[i*4+1] << 8u;
            tile |= data[i*4+2] << 16u;
            tile |= data[i*4+3] << 24u;

            layer.tileData[i].id = tile & ~(FLIPPED_DIAGONALLY_FLAG |
                                            FLIPPED_VERTICALLY_FLAG |
                                            FLIPPED_HORIZONTALLY_FLAG);
            layer.tileData[i].hflip = tile & FLIPPED_HORIZONTALLY_FLAG;
            layer.tileData[i].vflip = tile & FLIPPED_VERTICALLY_FLAG;
            layer.tileData[i].dflip = tile & FLIPPED_DIAGONALLY_FLAG;
        }

        map.layers.emplace_back(std::move(layer));
    }


    return map;
}

