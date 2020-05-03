//
// Created by felix on 29/04/2020.
//
#include <tinyxml2.h>
#include <filesystem>
#include <lyra/lyra.hpp>
#include <iostream>
#include <utility>
#include <base64.h>
#include <cctype>
#include <flatbuffers/flatbuffers.h>
#include <gear/fbs/generated/map_generated.h>
#include <fstream>

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

    std::string inputPath;
    std::string outFileName = "out";
    bool showHelp = false;
    bool printInputs = false;
    auto cli = lyra::cli_parser()
               | lyra::arg(inputPath, "input file")
                       ("input files").cardinality(1, std::numeric_limits<size_t>::max())
               | lyra::opt(outFileName, "output file name").required(1)
               ["-o"]["--output"]
               ("output file name")
               | lyra::help(showHelp);

    auto result = cli.parse({argc, argv});
    if (!result){
        std::cerr << result.errorMessage() << "\n";
        return 1;
    }

    if (showHelp)
    {
        std::cout << cli << std::endl;
        return 0;
    }


    flatbuffers::FlatBufferBuilder builder(2048);
    std::vector<flatbuffers::Offset<gear::bin::Layer>> layers;

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
            auto source = xTileset->Attribute("source");
            firstIds.emplace_back(firstId);
            tilesets.emplace_back(source);
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

            auto layer = gear::bin::CreateLayerDirect(builder, tilesets[tilesetId].c_str(), &tiles);
            layers.push_back(layer);
        }

        auto map = gear::bin::CreateMapDirect(builder, &layers);
        builder.Finish(map);

        auto buf = builder.GetBufferPointer();
        auto bufSize = builder.GetSize();
        std::ofstream ofs(outFileName, std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }


    return 0;
}