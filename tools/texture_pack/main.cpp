//
// Created by felix on 05/01/2020.
//

#include <lyra/lyra.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <ios>
#include <gear/assets_generated.h>
#include <stb_image.h>
#include <flatbuffers/hash.h>
#include "texture.h"
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

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

    auto reldir = fs::path(inputPath).parent_path();
    auto yaml = YAML::LoadFile(inputPath);
    auto source = yaml["image"].as<std::string>();
    auto format = yaml["format"].as<std::string>();

    flatbuffers::FlatBufferBuilder fbb(2048);

    int w, h, c;
    stbi_set_flip_vertically_on_load(1);
    auto imageData = (uint8_t*) stbi_load((reldir / source).c_str(), &w, &h, &c, 4);

    auto tex = gear::buildTexture(fbb, w, h, gear::assets::PixelFormat::RGBA8, imageData);

    stbi_image_free(imageData);

    std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
    entries.push_back(gear::assets::CreateAssetEntry(fbb, flatbuffers::HashFnv1<uint64_t>(fs::path(inputPath).stem().c_str()),
                                                     gear::assets::Asset::Texture, tex.Union()));

    auto assetVec = fbb.CreateVectorOfSortedTables(&entries);
    auto bundle = gear::assets::CreateBundle(fbb, assetVec);
    fbb.Finish(bundle);

    {
        auto buf = fbb.GetBufferPointer();
        auto bufSize = fbb.GetSize();
        std::ofstream ofs(outFileName, std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }

    return 0;
}