//
// Created by felix on 05/05/2020.
//

#include <flatbuffers/flatbuffers.h>
#include <fstream>
#include <tinyxml2.h>
#include <gear/tileset_generated.h>
#include <gear/assets_generated.h>
#include <ios>
#include <texture.h>
#include <filesystem>
#include <flatbuffers/hash.h>
#include <gear/common_generated.h>
#include <cuttlefish/Image.h>
#include <cuttlefish/Texture.h>
#include <iostream>

namespace xml = tinyxml2;
namespace fs = std::filesystem;
namespace cf = cuttlefish;

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

    cf::Image image;
    image.load((reldir/source).c_str(), cf::ColorSpace::Linear);
    if (!image.isValid()) {
        std::cerr << "error loading image " << source << "\n";
        return 1;
    }

    cf::Texture::Format cfFormat = cf::Texture::Format::R8G8B8A8;

    cf::Texture texture(cf::Texture::Dimension::Dim2D, image.width(), image.height());
    texture.setImage(image);
    if (!texture.convert(cfFormat, cf::Texture::Type::UNorm)) {
        std::cerr << "error converting image " << source << ".\n";
        if (!texture.imagesComplete()) {
            std::cerr << "\t image incomplete." << "\n";
        }
        return 1;
    }

    std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
    std::vector<flatbuffers::Offset<gear::assets::Ref>> references;

    std::string tilesetName = xTileset->Attribute("name");

    //Add texture asset
    const uint8_t* textureData = (uint8_t*)texture.data();
    size_t textureDataSize = texture.dataSize();
    auto textureBin = gear::buildTexture(builder, texture.width(), texture.height(), gear::assets::PixelFormat::RGBA8, &textureData, &textureDataSize, 1);
    auto textureName = tilesetName + "_texture";
    auto textureHash = flatbuffers::HashFnv1<uint64_t>(textureName.c_str());
    auto textureRef = gear::assets::CreateRef(builder, (uint8_t)gear::assets::Asset::Texture, textureHash);
    references.push_back(textureRef);
    entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(textureName.c_str()),gear::assets::Asset::Texture, textureBin.Union()));

    //add tileset asset
    auto tileset = gear::assets::CreateTileSet(builder, textureRef,
            xImage->IntAttribute("width"),
            xImage->IntAttribute("height"),
            xTileset->IntAttribute("tilewidth"),
            xTileset->IntAttribute("tileheight"),
            xTileset->IntAttribute("tilecount"),
            xTileset->IntAttribute("columns"));

    entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(tilesetName.c_str()), gear::assets::Asset::TileSet, tileset.Union()));
    auto assetVec = builder.CreateVectorOfSortedTables(&entries);
    auto refVec = builder.CreateVector(references);
    auto bundle = gear::assets::CreateBundle(builder, assetVec, 0, refVec);

    builder.Finish(bundle);

    std::ofstream s(out, std::ios::binary | std::ios::out);
    s.write((char*)builder.GetBufferPointer(), builder.GetSize());
}