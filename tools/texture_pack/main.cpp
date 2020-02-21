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
#include <regex>

namespace fs = std::filesystem;
namespace tp = gear::texture_pack;

int main(int argc, char* argv[]) {

    int pageWidth = 256, pageHeight = 256;
    std::vector<std::string> inputPaths;
    std::string outFileName = "out";
    bool showHelp = false;
    bool printInputs = false;
    auto cli = lyra::cli_parser()
            | lyra::arg(inputPaths, "input folders")
            ("folders to pack").cardinality(1, std::numeric_limits<size_t>::max())
            | lyra::opt(outFileName, "output file name").required(1)
            ["-o"]["--output"]
            ("output file name")
            | lyra::opt(pageWidth, "page width")
            ["-w"]["--width"]
            ("Width of page")
            | lyra::opt(pageHeight, "page height")
            ["-h"]["--height"]
            ("Height of page")
            | lyra::opt(printInputs)
            ["-w"]["--printInputs"]
            ("Use directory")
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


    std::vector<tp::SpriteDescriptor> descriptors;

    for(auto& d : inputPaths) {
        if (!fs::is_directory(d)) {
            std::cerr << d << " is not a directory.\n";
            return 1;
        }

        for(auto& p : fs::directory_iterator(d)) {
            descriptors.push_back(tp::getDescriptorFromPath(p));
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
            j["sprites"].push_back(o);
        }

        {
            std::ofstream ofs(outAtlasName);
            ofs << std::setw(4) << j << std::endl;
        }
    }

    return 0;
}