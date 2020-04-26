//
// Created by felix on 05/01/2020.
//

#include <lyra/lyra.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <filesystem>
#include "TexturePacker.h"
#include <fstream>
#include <ios>
#include <gear/fbs/generated/texture_atlas_generated.h>
#include <tinyxml2.h>


namespace fs = std::filesystem;
namespace tp = gear::texture_pack;
namespace xml = tinyxml2;

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
            | lyra::opt(printInputs)
            ["-p"]["--printInputs"]
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

    auto outTexName = (outFileName + ".png");
    auto outAtlasName = (outFileName + ".bin");

    int pageWidth = 256, pageHeight = 256;
    std::vector<tp::SpriteDescriptor> descriptors;

    {
        auto reldir = fs::path(inputPath).parent_path();

        xml::XMLDocument doc;
        doc.LoadFile(inputPath.c_str());
        xml::XMLElement* xTileSet = doc.FirstChildElement("tileset");

        for(xml::XMLElement* xTile = xTileSet->FirstChildElement("tile"); xTile;
        xTile = xTile->NextSiblingElement("tile")) {
            xml::XMLElement* xImage = xTile->FirstChildElement("image");
            auto source = xImage->Attribute("source");
            std::cout << source << std::endl;
            xml::XMLElement* xAnimation = xTile->FirstChildElement("animation");
            tp::SpriteDescriptor desc;
            desc.id = xTile->IntAttribute("id");
            desc.name = fs::path(source).stem();
            desc.origin.x = 0;
            desc.origin.y = 0;
            desc.images.emplace_back(reldir / source);
            descriptors.push_back(desc);
        }
    }

    std::cout << "done with loop" << std::endl;

    if (printInputs) {
        for(auto& spr : descriptors) {
            for(auto& img : spr.images) {
                std::cout << img << " ";
            }
        }
        return 0;
    }

    std::vector<tp::Sprite> sprites;
    sprites.reserve(descriptors.size());
    for(auto& desc : descriptors) {
        sprites.push_back(tp::loadSprite(desc));
    }

    {
        std::cout << "packing sprites" << std::endl;
        tp::packSprites(sprites, pageWidth, pageHeight);
    }

    {
        std::cout << "writing sprites" << std::endl;
        std::unique_ptr<tp::Pixel[]> data = std::make_unique<tp::Pixel[]>(pageWidth * pageHeight);
        tp::writeSprites(sprites, pageWidth, pageHeight, data.get());

        std::cout << "writing page" << std::endl;
        tp::writePageTexture(outTexName, pageWidth, pageHeight, data.get());
    }

    {
        for(auto& s : sprites) {
           tp::freeSprite(s);
        }
    }


    std::cout << "writing fb" << std::endl;
    flatbuffers::FlatBufferBuilder builder(2048);

    {
        std::vector<flatbuffers::Offset<gear::bin::Sprite>> spriteBins;
        for(auto& spr : sprites) {
            int maxWidth = 0, maxHeight = 0;
            std::vector<gear::bin::Region> regions_vector;
            regions_vector.reserve(spr.images.size());
            for(auto& img : spr.images) {
                regions_vector.emplace_back(gear::bin::ivec2(img.x, img.y), gear::bin::ivec2(img.width, img.height));
                maxWidth = img.width > maxWidth ? img.width : maxWidth;
                maxHeight = img.height > maxHeight ? img.height : maxHeight;
            }

            auto size = gear::bin::fvec2(maxWidth, maxHeight);
            auto origin = gear::bin::fvec2(spr.origin.x * maxWidth, spr.origin.y * maxHeight);
            auto sprite = gear::bin::CreateSpriteDirect(builder, spr.id, spr.name.c_str(), &size, &origin, &regions_vector);
            spriteBins.push_back(sprite);
        }

        auto texPathRel = fs::relative(outTexName, fs::path(outAtlasName).parent_path());
        auto atlas = gear::bin::CreateAtlasDirect(builder, texPathRel.c_str(), &spriteBins);
        builder.Finish(atlas);
    }


    {
        auto buf = builder.GetBufferPointer();
        auto bufSize = builder.GetSize();
        std::ofstream ofs(outAtlasName , std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }

    return 0;
}