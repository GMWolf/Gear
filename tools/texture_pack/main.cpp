//
// Created by felix on 05/01/2020.
//

#include <lyra/lyra.hpp>
#include <iostream>
#include <indicators/progress_bar.hpp>
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
    bool showHelp;
    auto cli = lyra::cli_parser()
            | lyra::arg(inputPaths, "input files")
            ("Files to pack").cardinality(1, std::numeric_limits<size_t>::max())
            | lyra::opt(pageWidth, "page width")
            ["-w"]["--width"]
            ("Width of page")
            | lyra::opt(pageHeight, "page height")
            ["-h"]["--height"]
            ("Height of page")
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

    std::vector<tp::Sprite> sprites;
    {
        indicators::ProgressBar bar;
        bar.set_prefix_text("Loading images");

        sprites.reserve(inputPaths.size());
        for(int i = 0; i < inputPaths.size(); i++) {
            auto& path = inputPaths[i];
            int w, h, c;
            unsigned char* data = stbi_load(path.c_str(), &w, &h, &c, 4);
            sprites.push_back({
               fs::path(path).filename().string(),
               0, 0,
               (unsigned short)w, (unsigned short)h,
               (tp::Pixel*)data
            });
            bar.set_progress((i * 100.0f) / inputPaths.size());
        }

        bar.mark_as_completed();
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