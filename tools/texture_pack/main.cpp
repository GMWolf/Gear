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
#include <nlohmann/json.hpp>
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;
namespace tp = gear::texture_pack;

int main(int argc, char* argv[]) {

    std::vector<std::string> inputPaths;
    std::string outFileName = "out";
    bool showHelp = false;
    bool printInputs = false;
    auto cli = lyra::cli_parser()
            | lyra::arg(inputPaths, "input folders")
            ("input files").cardinality(1, std::numeric_limits<size_t>::max())
            | lyra::opt(outFileName, "output file name").required(1)
            ["-o"]["--output"]
            ("output file name")
            | lyra::opt(printInputs)
            ["-w"]["--printInputs"]
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
    auto outAtlasName = (outFileName + ".json");


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
            desc.origin.x = n["origin_x"].as<float>();
            desc.origin.y = n["origin_y"].as<float>();
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

    {
        nlohmann::json j;
        j["texture"] = outTexName;
        j["sprites"] = {};
        for(auto& spr : sprites) {
            nlohmann::json o;
            o["name"] = spr.name;
            o["subimages"] = {};
            int maxWidth = 0, maxHeight = 0;
            for(auto& img : spr.images) {
                nlohmann::json s;
                s["x"] = img.x;
                s["y"] = img.y;
                s["w"] = img.width;
                s["h"] = img.height;
                o["subimages"].push_back(s);
                maxWidth = img.width > maxWidth ? img.width : maxWidth;
                maxHeight = img.height > maxHeight ? img.height : maxHeight;
            }
            o["size"]["x"] = maxWidth;
            o["size"]["y"] = maxHeight;
            o["origin"]["x"] = spr.origin.x * maxWidth;
            o["origin"]["y"] = spr.origin.y * maxHeight;
            j["sprites"].push_back(o);
        }

        {
            std::ofstream ofs(outAtlasName);
            ofs << std::setw(4) << j << std::endl;
        }
    }

    return 0;
}