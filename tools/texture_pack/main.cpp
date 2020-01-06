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

namespace fs = std::filesystem;
namespace tp = gear::texture_pack;

int main(int argc, char* argv[]) {

    int pageWidth = 256, pageHeight = 256;
    std::string inputDir;
    bool showHelp;
    auto cli = lyra::cli_parser()
            | lyra::arg(inputDir, "input")
              ("Directory to pack")
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


    fs::path dir = fs::absolute(inputDir);
    if (!fs::is_directory(dir)) {
        std::cerr << "No such directory " << dir << "\n";
        return -1;
    }


    std::vector<tp::Sprite> sprites;
    {
        indicators::ProgressBar bar;
        bar.set_prefix_text("Loading images");

        auto dt = fs::directory_iterator(dir);

        std::vector<fs::path> spritePaths;
        for (auto &p : dt) {
            if (p.is_regular_file()) {
                if (p.path().extension() == ".jpg"
                | p.path().extension() == ".png") {
                    spritePaths.push_back(p.path());
                }
            }
        }

        sprites.reserve(spritePaths.size());
        for(int i = 0; i < spritePaths.size(); i++) {
            auto& path = spritePaths[i];
            int w, h, c;
            unsigned char* data = stbi_load(path.string().c_str(), &w, &h, &c, 4);
            sprites.push_back({
               path.filename().string(),
               0, 0,
               (unsigned short)w, (unsigned short)h,
               (tp::Pixel*)data
            });
            bar.set_progress((i * 100.0f) / spritePaths.size());
        }

        bar.mark_as_completed();
    }


    {
        tp::packSprites(sprites.data(), sprites.size(), pageWidth, pageHeight);
    }

    {
        std::unique_ptr<tp::Pixel[]> data = std::make_unique<tp::Pixel[]>(pageWidth * pageHeight);
        tp::writeSprites(sprites.data(), sprites.size(), pageWidth, pageHeight, data.get());

        stbi_write_png("out.png", pageWidth, pageHeight, 4, data.get(), pageWidth * sizeof(tp::Pixel));
    }


    return 0;
}