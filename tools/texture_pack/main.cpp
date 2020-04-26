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
#include <yaml-cpp/yaml.h>
#include <gear/fbs/generated/texture_atlas_generated.h>

namespace fs = std::filesystem;
namespace tp = gear::texture_pack;

int main(int argc, char* argv[]) {

    std::vector<std::string> inputPaths;
    std::string outFileName = "out";
    bool showHelp = false;
    bool printInputs = false;
    auto cli = lyra::cli_parser()
            | lyra::arg(inputPaths, "input file")
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

    for(auto& inputPath : inputPaths) {

        auto config = YAML::LoadFile(inputPath);
        auto sprites = config["sprites"];
        pageWidth = config["width"].as<int>();
        pageHeight = config["height"].as<int>();

        auto pathRelDir = fs::path(inputPath).parent_path();

        assert(sprites.IsSequence());
        for(const auto& n : sprites) {
            assert(n.IsMap());
            tp::SpriteDescriptor desc;
            desc.name = n["name"].as<std::string>();
            desc.origin.x = n["origin_x"] ? n["origin_x"].as<float>() : 0.0f;
            desc.origin.y = n["origin_y"] ? n["origin_y"].as<float>() : 0.0f;
            if (n["images"].IsSequence()) {
                for(const auto& i : n["images"]) {
                    desc.images.emplace_back(pathRelDir / i.as<std::string>());
                }
            } else {
                desc.images.emplace_back(pathRelDir / n["images"].as<std::string>());
            }

            descriptors.push_back(desc);
        }
    }

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
        tp::packSprites(sprites, pageWidth, pageHeight);
    }

    {
        std::unique_ptr<tp::Pixel[]> data = std::make_unique<tp::Pixel[]>(pageWidth * pageHeight);
        tp::writeSprites(sprites, pageWidth, pageHeight, data.get());

        tp::writePageTexture(outTexName, pageWidth, pageHeight, data.get());
    }

    {
        for(auto& s : sprites) {
           tp::freeSprite(s);
        }
    }


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
            auto sprite = gear::bin::CreateSpriteDirect(builder, spr.name.c_str(), &size, &origin, &regions_vector);
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