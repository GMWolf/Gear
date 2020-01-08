//
// Created by felix on 05/01/2020.
//

#include <lyra/lyra.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <stb_image.h>
#include <stb_image_write.h>
#include <string>
#include <filesystem>
#include "TexturePacker.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace fs = std::filesystem;
namespace tp = gear::texture_pack;

int main(int argc, char* argv[]) {

    int pageWidth = 256, pageHeight = 256;
    std::vector<std::string> inputPaths;
    std::string outFileName = "out";
    bool showHelp = false;
    bool directoryMode = false;
    auto cli = lyra::cli_parser()
            | lyra::arg(inputPaths, "input files")
            ("Files to pack").cardinality(1, std::numeric_limits<size_t>::max())
            | lyra::opt(outFileName, "output file name").required(1)
            ["-o"]["--output"]
            ("output file name")
            | lyra::opt(pageWidth, "page width")
            ["-w"]["--width"]
            ("Width of page")
            | lyra::opt(pageHeight, "page height")
            ["-h"]["--height"]
            ("Height of page")
            | lyra::opt(directoryMode)
            ["-d"]["--directory"]
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

    std::vector<fs::path> inputFiles;

    if (directoryMode) {
        for(auto& d : inputPaths) {
            for(auto& f : fs::directory_iterator(d)) {
                if (f.path().extension() == ".png") {
                    inputFiles.push_back(f.path());
                }
            }
        }
    } else {
        inputFiles.reserve(inputPaths.size());
        for(auto& s : inputPaths) {
            inputFiles.emplace_back(s);
        }
    }

    std::vector<tp::Sprite> sprites;
    {
        sprites.reserve(inputPaths.size());
        for(auto & path : inputFiles) {
            int w, h, c;
            std::string pathStr = path.string();
            std::cout << "loading " << pathStr << std::endl;
            unsigned char* data = stbi_load(pathStr.c_str(), &w, &h, &c, 4);
            sprites.push_back({
               fs::path(path).filename().stem().string(),
               0, 0,
               (unsigned short)w, (unsigned short)h,
               (tp::Pixel*)data
            });
        }
    }

    {
        tp::packSprites(sprites.data(), sprites.size(), pageWidth, pageHeight);
    }

    {
        std::unique_ptr<tp::Pixel[]> data = std::make_unique<tp::Pixel[]>(pageWidth * pageHeight);
        tp::writeSprites(sprites.data(), sprites.size(), pageWidth, pageHeight, data.get());

        stbi_write_png(outTexName.c_str(), pageWidth, pageHeight, 4, data.get(), pageWidth * sizeof(tp::Pixel));
    }

    {
        for(auto& s : sprites) {
            stbi_image_free((void *) s.data);
            s.data = nullptr;
        }
    }

    {
        nlohmann::json j;
        j["texture"] = outTexName;
        j["sprites"] = {};
        for(auto& spr : sprites) {
            nlohmann::json o;
            o["x"] = spr.x;
            o["y"] = spr.y;
            o["w"] = spr.width;
            o["h"] = spr.height;
            o["name"] = spr.name;
            j["sprites"].push_back(o);
        }

        {
            std::ofstream ofs(outAtlasName);
            ofs << std::setw(4) << j << std::endl;
        }
    }

    return 0;
}