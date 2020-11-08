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
#include <flatbuffers/hash.h>
#include "texture.h"
#include <yaml-cpp/yaml.h>
#include <cuttlefish/Image.h>
#include <cuttlefish/Texture.h>

namespace fs = std::filesystem;
namespace cf = cuttlefish;

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

    cf::Image image;
    image.load((reldir/source).c_str(), cf::ColorSpace::Linear);
    image.flipVertical();
    if (!image.isValid()) {
        std::cerr << "error loading image " << source << "\n";
        return 1;
    }

    cf::Texture::Format cfFormat;
    gear::assets::PixelFormat gearFormat = gear::assets::PixelFormat::Invalid;

    for(auto f : gear::assets::EnumValuesPixelFormat()) {
        if (strcmp(gear::assets::EnumNamePixelFormat(f), format.c_str()) == 0) {
            gearFormat = f;
            break;
        }
    }

    switch (gearFormat) {
        case gear::assets::PixelFormat::Invalid:
            std::cerr << "Invalid pixel format " << format << ".\n";
            return 1;
        case gear::assets::PixelFormat::R8:
            cfFormat = cf::Texture::Format::R8;
            break;
        case gear::assets::PixelFormat::RGB8:
            cfFormat = cf::Texture::Format::R8G8B8;
            break;
        case gear::assets::PixelFormat::RGBA8:
            cfFormat = cf::Texture::Format::R8G8B8A8;
            break;
        case gear::assets::PixelFormat::BC7:
            cfFormat = cf::Texture::Format::BC7;
            break;
    }


    cf::Texture texture(cf::Texture::Dimension::Dim2D, image.width(), image.height());

    texture.setImage(image);
    if (!texture.convert(cfFormat, cf::Texture::Type::UNorm, cf::Texture::Quality::Normal )) {
        std::cerr << "error converting image " << source << ".\n";
        if (!texture.imagesComplete()) {
            std::cerr << "\t image incomplete." << "\n";
        }
        if (!cf::Texture::isFormatValid(cfFormat, cf::Texture::Type::UNorm)) {
            std::cerr << "\t format invalid." << "\n";
        }
        return 1;
    }

    auto tex = gear::buildTexture(fbb, image.width(), image.height(), gearFormat, (uint8_t*)texture.data(), texture.dataSize());

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