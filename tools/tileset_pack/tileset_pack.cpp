//
// Created by felix on 05/05/2020.
//

#include <flatbuffers/flatbuffers.h>
#include <fstream>
#include <tinyxml2.h>
#include <gear/fbs/generated/tileset_generated.h>
#include <gear/fbs/generated/assets_generated.h>
#include <ios>


namespace xml = tinyxml2;

int main(int charc, char* argv[]) {

    auto in = argv[1];
    auto out = argv[2];

    xml::XMLDocument doc;
    doc.LoadFile(in);


    auto xTileset = doc.FirstChildElement("tileset");


    flatbuffers::FlatBufferBuilder builder(2048);

    auto textureName = builder.CreateString(xTileset->FirstChildElement("image")->Attribute("source"));

    gear::assets::TileSetBuilder tilesetBuilder(builder);
    tilesetBuilder.add_texture(textureName);
    tilesetBuilder.add_columns(xTileset->IntAttribute("columns"));
    tilesetBuilder.add_tileCount(xTileset->IntAttribute("tileCount"));
    tilesetBuilder.add_tileWidth(xTileset->IntAttribute("tileWidth"));
    tilesetBuilder.add_tileHeight(xTileset->IntAttribute("tileHeight"));
    auto tileset = tilesetBuilder.Finish();

    std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
    entries.push_back(gear::assets::CreateAssetEntryDirect(builder, xTileset->Attribute("name"), gear::assets::Asset_TileSet, tileset.Union()));
    auto bundle = gear::assets::CreateBundleDirect(builder, &entries);

    builder.Finish(bundle);

    std::ofstream s(out, std::ios::binary | std::ios::out);
    s.write((char*)builder.GetBufferPointer(), builder.GetSize());
}