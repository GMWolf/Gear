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
#include <fstream>
#include <gear/fbs/generated/assets_generated.h>

namespace fs = std::filesystem;

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
    std::string binOut = outFileName + ".bin";

    const auto config = YAML::LoadFile(inputPath);

    auto bitmapWidth = config["bitmap_width"].as<int>();
    auto bitmapHeight = config["bitmap_height"].as<int>();
    auto lineHeight = config["line_height"].as<float>();
    auto rangeStart = config["range_start"].as<int>();
    auto rangeCount = config["range_count"].as<int>();
    auto name = config["name"].as<std::string>();

    auto pathRelDir = fs::path(inputPath).parent_path();
    auto file = pathRelDir / config["font"].as<std::string>();

    const unsigned char* ttf_data = (unsigned char*)stb_file(const_cast<char*>(file.c_str()), nullptr);

    if (!ttf_data) {
        std::cerr << "could not open " << file.c_str() << "\n";
        return 1;
    }

    std::vector<uint32_t> bitmap(bitmapWidth * bitmapHeight, 0);

    stbtt_packedchar pdata[256];
    memset(pdata, 0, sizeof(pdata));
    stbtt_pack_context spc;
    stbtt_PackBegin(&spc, (unsigned char*)bitmap.data(), bitmapWidth, bitmapHeight, 0, 1, nullptr);
    stbtt_PackSetOversampling(&spc, 1, 1);
    stbtt_PackFontRange(&spc, ttf_data, 0, lineHeight, rangeStart, rangeCount, pdata);
    stbtt_PackEnd(&spc);

    stbi_write_png(bitmapOut.c_str(), bitmapWidth, bitmapHeight, 1, bitmap.data(), 0);

    flatbuffers::FlatBufferBuilder builder(2048);
    {
        std::vector<gear::assets::Glyph> glyphs;
        glyphs.reserve(rangeCount);
        for(int i = 0; i < rangeCount; i++) {
            gear::assets::Glyph glyph {
                pdata[i].x0,
                static_cast<uint16_t>(bitmapHeight - pdata[i].y1),
                pdata[i].x1,
                static_cast<uint16_t>(bitmapHeight - pdata[i].y0),
                pdata[i].xadvance,
                pdata[i].xoff,
                pdata[i].yoff,
                pdata[i].xoff2,
                pdata[i].yoff2
            };
            glyphs.push_back(glyph);
        }
        auto bitmapPathRelative = fs::relative(bitmapOut, fs::path(binOut).parent_path());
        auto tex = gear::assets::CreateTextureDirect(builder, bitmapWidth, bitmapHeight, &bitmap);
        auto texName = name + "_texture";

        auto font = gear::assets::CreateFontDirect(builder, texName.c_str(), rangeStart, rangeCount, &glyphs);

        std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
        entries.push_back(gear::assets::CreateAssetEntryDirect(builder, texName.c_str(), gear::assets::Asset_Texture, tex.Union()));
        entries.push_back(gear::assets::CreateAssetEntryDirect(builder, name.c_str(), gear::assets::Asset_Font, font.Union()));
        auto bundle = gear::assets::CreateBundleDirect(builder, &entries);

        builder.Finish(bundle);
    }

    {
        auto buf = builder.GetBufferPointer();
        auto bufSize = builder.GetSize();
        std::ofstream ofs(binOut, std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }
    return 0;
}