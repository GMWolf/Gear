//
// Created by felix on 23/02/2020.
//

#include <cstdlib>
#include <lyra/lyra.hpp>
#include <iostream>
#include <stb.h>
#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <stb_image_write.h>
#include <vector>
#include <string>
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>

namespace fs = std::filesystem;
namespace nj = nlohmann;

int main(int argc, char* argv[]) {

    std::string inputPath;
    std::string outFileName = "out";
    bool showHelp = false;
    bool printInputs = false;
    auto cli = lyra::cli_parser()
               | lyra::arg(inputPath, "input file")
                       ("input files")
               | lyra::opt(outFileName, "output file name").required(1)
               ["-o"]["--output"]
                       ("output file name")
               | lyra::opt(printInputs)
               ["-p"]["--printInputs"]
               | lyra::help(showHelp);
    auto result = cli.parse({argc, argv});

    if (!result) {
        std::cerr << result.errorMessage() << "\n";
        return 1;
    }

    if (showHelp)
    {
        std::cout << cli << std::endl;
        return 0;
    }

    std::string bitmapOut = outFileName + ".png";
    std::string jsonOut = outFileName + ".json";

    const auto config = YAML::LoadFile(inputPath);

    auto bitmapWidth = config["bitmap_width"].as<int>();
    auto bitmapHeight = config["bitmap_height"].as<int>();
    auto lineHeight = config["line_height"].as<float>();
    auto rangeStart = config["range_start"].as<int>();
    auto rangeCount = config["range_count"].as<int>();

    auto pathRelDir = fs::path(inputPath).parent_path();
    auto file = pathRelDir / config["font"].as<std::string>();

    const unsigned char* ttf_data = (unsigned char*)stb_file(const_cast<char*>(file.c_str()), nullptr);

    if (!ttf_data) {
        std::cerr << "could not open " << file.c_str() << "\n";
        return 1;
    }

    auto* bitmap = static_cast<unsigned char*>(calloc(bitmapWidth * bitmapHeight, sizeof(unsigned char)));

    stbtt_packedchar pdata[256];
    memset(pdata, 0, sizeof(pdata));
    stbtt_pack_context spc;
    stbtt_PackBegin(&spc, bitmap, bitmapWidth, bitmapHeight, 0, 1, nullptr);
    stbtt_PackSetOversampling(&spc, 1, 1);
    stbtt_PackFontRange(&spc, ttf_data, 0, lineHeight, rangeStart, rangeCount, pdata);
    stbtt_PackEnd(&spc);
    stbi_write_png(bitmapOut.c_str(), bitmapWidth, bitmapHeight, 1, bitmap, 0);


    nj::json j;
    j["texture"] = bitmapOut;
    j["glyphs"] = {};
    j["rangeStart"] = rangeStart;
    j["rangeCount"] = rangeCount;
    auto& glyphs = j["glyphs"];
    for(int i = 0; i < rangeCount; i++) {
        nj::json g;
        g["x0"] = pdata[i].x0;
        g["x1"] = pdata[i].x1;
        g["y0"] = pdata[i].y0;
        g["y1"] = pdata[i].y1;
        g["xadvance"] = pdata[i].xadvance;
        g["xoff"] = pdata[i].xoff;
        g["yoff"] = pdata[i].yoff;
        g["xoff2"] = pdata[i].xoff2;
        g["yoff2"] = pdata[i].yoff2;
        glyphs.push_back(g);
    }

    {
        std::ofstream ofs(jsonOut);
        ofs << std::setw(4) << j << std::endl;
    }
}