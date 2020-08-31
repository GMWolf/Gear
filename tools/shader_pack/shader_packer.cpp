//
// Created by felix on 03/05/2020.
//

#include <lyra/lyra.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <fstream>
#include <flatbuffers/flatbuffers.h>
#include <gear/fbs/generated/assets_generated.h>
#include <iostream>
#include <filesystem>
#include <flatbuffers/hash.h>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    std::string inputPath;
    std::string outFileName = "out";
    auto cli = lyra::cli_parser()
               | lyra::arg(inputPath, "input file")
                       ("input files")
               | lyra::opt(outFileName, "output file name").required(1)
               ["-o"]["--output"] ("output file name");
    auto result = cli.parse({argc, argv});

    if (!result) {
        std::cerr << result.errorMessage() << "\n";
        return 1;
    }



    const auto config = YAML::LoadFile(inputPath);

    auto name = config["name"].as<std::string>();
    auto vertexFileName = config["vertex"].as<std::string>();
    auto fragmentFileName = config["fragment"].as<std::string>();

    auto pathRelDir = fs::path(inputPath).parent_path();

    auto vertexPath = pathRelDir / vertexFileName;
    auto fragmentPath = pathRelDir / fragmentFileName;

    std::ifstream vertexStream(vertexPath);
    std::string vertexText((std::istreambuf_iterator<char>(vertexStream)),
                    std::istreambuf_iterator<char>());

    std::ifstream fragmentStream(fragmentPath);
    std::string fragmentText((std::istreambuf_iterator<char>(fragmentStream)),
                           std::istreambuf_iterator<char>());

    std::string header = "#version 330 core\n";

    flatbuffers::FlatBufferBuilder builder(1000 + fragmentText.size() + vertexText.size());
    auto vertTextBin = builder.CreateString(header + vertexText);
    auto fragTextBin = builder.CreateString(header + fragmentText);
    gear::assets::ShaderBuilder shaderBuilder(builder);
    shaderBuilder.add_vertex(vertTextBin);
    shaderBuilder.add_fragment(fragTextBin);
    auto shader = shaderBuilder.Finish();

    std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
    entries.push_back(gear::assets::CreateAssetEntry(builder, flatbuffers::HashFnv1<uint64_t>(name.c_str()), gear::assets::Asset_Shader, shader.Union()));
    auto assetVec = builder.CreateVectorOfSortedTables(&entries);
    auto bundle = gear::assets::CreateBundle(builder, assetVec);
    builder.Finish(bundle);

    {
        auto buf = builder.GetBufferPointer();
        auto bufSize = builder.GetSize();
        std::ofstream ofs(outFileName, std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }

    return 0;
}