//
// Created by felix on 29/04/2020.
//
#include <tinyxml2.h>
#include <filesystem>
#include <iostream>
#include <utility>
#include <base64.h>
#include <cctype>
#include <flatbuffers/flatbuffers.h>
#include <zstd.h>
#include <gear/fbs/generated/assets_generated.h>
#include <fstream>
#include <flatbuffers/hash.h>

namespace fs = std::filesystem;
namespace xml = tinyxml2;


const unsigned TILE_FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned TILE_FLIPPED_VERTICALLY_FLAG   = 0x40000000;
const unsigned TILE_FLIPPED_DIAGONALLY_FLAG   = 0x20000000;
const unsigned TILE_ID_MASK = ~(TILE_FLIPPED_HORIZONTALLY_FLAG | TILE_FLIPPED_VERTICALLY_FLAG | TILE_FLIPPED_DIAGONALLY_FLAG);

unsigned int tile_getId(unsigned int tile) {
    return tile & TILE_ID_MASK;
}

int findTileset(int tile, const std::vector<int>& firstIds) {
    unsigned int id = tile_getId(tile);
    for(size_t i = firstIds.size() - 1; i >= 0; i--) {
        if (firstIds[i] <= id) {
            return i;
        }
    }
    return 0;
}


int main(int argc, char* argv[]) {

    std::string inputPath = argv[1];
    std::string outFileName = argv[2];

    auto relpath = fs::path(inputPath).parent_path();

    flatbuffers::FlatBufferBuilder builder(2048);
    std::vector<flatbuffers::Offset<gear::assets::Layer>> layers;

    {
        xml::XMLDocument doc;
        doc.LoadFile(inputPath.c_str());
        auto xMap = doc.FirstChildElement("map");

        std::vector<int> firstIds;
        std::vector<std::string> tilesets;

        //Get tilesets
        for(auto xTileset = xMap->FirstChildElement("tileset"); xTileset;
        xTileset = xTileset->NextSiblingElement("tileset")) {
            int firstId = xTileset->IntAttribute("firstgid");
            auto source = relpath / xTileset->Attribute("source");
            xml::XMLDocument tilesetDoc;
            tilesetDoc.LoadFile(source.c_str());
            auto name = tilesetDoc.FirstChildElement("tileset")->Attribute("name");
            firstIds.emplace_back(firstId);
            tilesets.emplace_back(name);
        }

        //get layers
        for(auto xLayer = xMap->FirstChildElement("layer"); xLayer;
        xLayer = xLayer->NextSiblingElement("layer")) {
            auto xData = xLayer->FirstChildElement("data");
            std::string data64 = xData->GetText();
            data64.erase(data64.begin(), std::find_if(data64.begin(), data64.end(), [](char c){return !std::isspace(c);}));
            auto data = base64_decode(data64);

            //get first tile;
            uint32_t tile = data[0] | data[1] << 8u || data[2] << 16u | data[3] << 24u;
            auto tilesetId = findTileset(tile, firstIds);
            auto tilesetFirstGid = firstIds[tilesetId];

            std::vector<uint32_t> tiles;

            for(int i = 0; i < data.size()/4; i++) {
                uint32_t tile = data[i*4] | data[i*4+1] << 8u | data[i*4+2] << 16u | data[i*4+3] << 24u;
                if (findTileset(tile, firstIds) != tilesetId) {
                    return 1;
                }
                uint32_t id = tile_getId(tile) - tilesetFirstGid;
                tiles.push_back(id);
            }

            auto layerName = xLayer->Attribute("name");
            auto layerWidth = xLayer->IntAttribute("width");
            auto layerHeight = xLayer->IntAttribute("height");

            auto layer = gear::assets::CreateLayerDirect(builder, layerName, flatbuffers::HashFnv1<uint64_t>(tilesets[tilesetId].c_str()),layerWidth, layerHeight, &tiles);
            layers.push_back(layer);
        }

        auto map = gear::assets::CreateMapDirect(builder, &layers);

        std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
        entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>("map"), gear::assets::Asset_Map, map.Union()));
        auto assetVec = builder.CreateVectorOfSortedTables(&entries);
        auto bundle = gear::assets::CreateBundle(builder, assetVec);
        builder.Finish(bundle);

        auto buf = builder.GetBufferPointer();
        auto bufSize = builder.GetSize();
        std::ofstream ofs(outFileName, std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }


    return 0;
}