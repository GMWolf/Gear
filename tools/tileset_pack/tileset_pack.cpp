//
// Created by felix on 05/05/2020.
//

#include <flatbuffers/flatbuffers.h>
#include <fstream>
#include <tinyxml2.h>
#include <gear/fbs/generated/tileset_generated.h>
#include <gear/fbs/generated/assets_generated.h>
#include <ios>
#include <stb_image.h>
#include <texture.h>
#include <filesystem>
#include <flatbuffers/hash.h>
#include <gear/fbs/generated/common_generated.h>

namespace xml = tinyxml2;
namespace fs = std::filesystem;

int main(int charc, char* argv[]) {

    auto in = argv[1];
    auto out = argv[2];

    xml::XMLDocument doc;
    doc.LoadFile(in);

    auto xTileset = doc.FirstChildElement("tileset");


    flatbuffers::FlatBufferBuilder builder(2048);

    auto reldir = fs::path(in).parent_path();
    auto xImage = xTileset->FirstChildElement("image");
    auto source = xImage->Attribute("source");

    int w, h, c;
    //stbi_set_flip_vertically_on_load(1);
    auto imageData = (uint8_t*) stbi_load((reldir / source).c_str(), &w, &h, &c, 4);


    std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
    std::vector<flatbuffers::Offset<gear::assets::Ref>> references;

    std::string tilesetName = xTileset->Attribute("name");

    //Add texture asset
    auto texture = gear::buildTexture(builder, w, h, gear::assets::PixelFormat_RGBA8, imageData);
    auto textureName = tilesetName + "_texture";
    auto textureHash = flatbuffers::HashFnv1<uint64_t>(textureName.c_str());
    auto textureRef = gear::assets::CreateRef(builder, gear::assets::Asset_Texture, textureHash);
    references.push_back(textureRef);
    entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(textureName.c_str()),gear::assets::Asset_Texture, texture.Union()));

    //add tileset asset
    auto tileset = gear::assets::CreateTileSet(builder, textureRef,
            xImage->IntAttribute("width"),
            xImage->IntAttribute("height"),
            xTileset->IntAttribute("tilewidth"),
            xTileset->IntAttribute("tileheight"),
            xTileset->IntAttribute("tilecount"),
            xTileset->IntAttribute("columns"));

    entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(tilesetName.c_str()), gear::assets::Asset_TileSet, tileset.Union()));
    auto assetVec = builder.CreateVectorOfSortedTables(&entries);
    auto refVec = builder.CreateVector(references);
    auto bundle = gear::assets::CreateBundle(builder, assetVec, 0, refVec);

    builder.Finish(bundle);

    std::ofstream s(out, std::ios::binary | std::ios::out);
    s.write((char*)builder.GetBufferPointer(), builder.GetSize());
}