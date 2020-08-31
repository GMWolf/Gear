//
// Created by felix on 23/02/2020.
//

#include <cstdlib>
#include <lyra/lyra.hpp>
#include <iostream>
#include <stb.h>
#include <stb_rect_pack.h>
#include <stb_truetype.h>
#include <vector>
#include <string>
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <fstream>
#include <gear/fbs/generated/assets_generated.h>
#include <texture.h>
#include <flatbuffers/hash.h>

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

    stbtt_packedchar pdata[256];
    std::vector<unsigned char> bitmap(bitmapWidth * bitmapHeight, 0);
    {
        memset(pdata, 0, sizeof(pdata));
        stbtt_pack_context spc;
        stbtt_PackBegin(&spc, bitmap.data(), bitmapWidth, bitmapHeight, 0, 1, nullptr);
        stbtt_PackSetOversampling(&spc, 1, 1);
        stbtt_PackFontRange(&spc, ttf_data, 0, lineHeight, rangeStart, rangeCount, pdata);
        stbtt_PackEnd(&spc);

        //flip vertically (for use in opengl)
        {
            std::vector<unsigned char> rowBuffer(bitmapWidth);

            for(int i = 0; i < bitmapHeight / 2; i++) {
                auto row1 = bitmap.data() + (i * bitmapWidth);
                auto row2 = bitmap.data() + ((bitmapHeight - i - 1) * bitmapWidth);
                memcpy(rowBuffer.data(), row1, bitmapWidth);
                memcpy(row1, row2, bitmapWidth);
                memcpy(row2, rowBuffer.data(), bitmapWidth);
            }
        }
    }

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
        auto tex = gear::buildTexture(builder, bitmapWidth, bitmapHeight, gear::assets::PixelFormat_R8, bitmap.data());
        auto texName = name + "_texture";
        auto texNameHash = flatbuffers::HashFnv1<uint64_t>(texName.c_str());
        auto texRef = gear::assets::CreateTextureRef(builder, texNameHash, tex);

        auto font = gear::assets::CreateFontDirect(builder, texRef, rangeStart, rangeCount, &glyphs);

        std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
        entries.reserve(2);
        entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(texName.c_str()), gear::assets::Asset_Texture, tex.Union()));
        entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(name.c_str()), gear::assets::Asset_Font, font.Union()));
        auto assetVec = builder.CreateVectorOfSortedTables(&entries);
        auto bundle = gear::assets::CreateBundle(builder, assetVec);

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