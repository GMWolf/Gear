//
// Created by felix on 03/05/2020.
//

#include <lyra/lyra.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <fstream>
#include <flatbuffers/flatbuffers.h>
#include <gear/assets_generated.h>
#include <iostream>
#include <filesystem>
#include <flatbuffers/hash.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

namespace fs = std::filesystem;


std::string preprocessShader(const std::string& sourceName, shaderc_shader_kind kind, const std::string& source, const shaderc::CompileOptions& options)
{
    shaderc::Compiler compiler;

    auto result = compiler.PreprocessGlsl(source, kind, sourceName.c_str(), options);
    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        std::cerr << result.GetErrorMessage();
        return "";
    }

    return {result.cbegin(), result.cend()};
}


std::vector<uint32_t> compileShader(const std::string& sourceName, shaderc_shader_kind kind, const std::string& source, const shaderc::CompileOptions& options)
{
    shaderc::Compiler compiler;

    auto result = compiler.CompileGlslToSpv(source, kind, sourceName.c_str(), options);
    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        std::cerr << result.GetErrorMessage();
        return {};
    }

    return {result.cbegin(), result.cend()};
}


void createReflectionData( flatbuffers::FlatBufferBuilder& fbb,  const std::vector<uint32_t>& data,
                           std::vector<flatbuffers::Offset<gear::assets::ShaderResource>>& resourceVec) {
    spirv_cross::Compiler compiler(data);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();
    for(auto &resource : resources.sampled_images) {
        auto name = fbb.CreateString(resource.name);
        gear::assets::ShaderResourceBuilder shaderResourceBuilder(fbb);
        shaderResourceBuilder.add_name(name);
        shaderResourceBuilder.add_binding(compiler.get_decoration(resource.id, spv::DecorationBinding));
        shaderResourceBuilder.add_type(gear::assets::ShaderResourceType::sampler);
        resourceVec.push_back(shaderResourceBuilder.Finish());
    }

    for(auto &resource : resources.uniform_buffers) {
        auto name = fbb.CreateString(resource.name);
        gear::assets::ShaderResourceBuilder shaderResourceBuilder(fbb);
        shaderResourceBuilder.add_name(name);
        shaderResourceBuilder.add_binding(compiler.get_decoration(resource.id, spv::DecorationBinding));
        shaderResourceBuilder.add_type(gear::assets::ShaderResourceType::uniform_buffer);
        resourceVec.push_back(shaderResourceBuilder.Finish());
    }

}


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
    auto version = config["version"].as<int>();

    auto pathRelDir = fs::path(inputPath).parent_path();

    auto vertexPath = pathRelDir / vertexFileName;
    auto fragmentPath = pathRelDir / fragmentFileName;

    std::ifstream vertexStream(vertexPath);
    std::string vertexSource((std::istreambuf_iterator<char>(vertexStream)),
                    std::istreambuf_iterator<char>());

    std::ifstream fragmentStream(fragmentPath);
    std::string fragmentSource((std::istreambuf_iterator<char>(fragmentStream)),
                           std::istreambuf_iterator<char>());

    shaderc::CompileOptions options;
    options.SetGenerateDebugInfo();
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    std::string header = "#version " + std::to_string(version) + " core\n";
    options.SetForcedVersionProfile(version, shaderc_profile_core);
    options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

    auto vertexText = preprocessShader(vertexFileName, shaderc_vertex_shader, header + vertexSource, options);
    auto fragmentText = preprocessShader(fragmentFileName, shaderc_fragment_shader, header + fragmentSource, options);

    flatbuffers::FlatBufferBuilder fbb(1000 + fragmentText.size() + vertexText.size());

    flatbuffers::Offset<gear::assets::Shader> shader;
    if (config["compile"].IsDefined())
    {
        auto vertexTextModule = compileShader(vertexFileName, shaderc_vertex_shader, vertexText, options);
        auto fragmentTextModule = compileShader(fragmentFileName, shaderc_fragment_shader, fragmentText, options);


        auto vertTextOffset = gear::assets::CreateShaderTextDirect(fbb, nullptr, &vertexTextModule);
        auto fragTextOffset = gear::assets::CreateShaderTextDirect(fbb, nullptr, &fragmentTextModule);
        std::vector<flatbuffers::Offset<gear::assets::ShaderResource>> resources;
        createReflectionData(fbb, vertexTextModule, resources);
        createReflectionData(fbb, fragmentTextModule, resources);
        auto fbbResources = fbb.CreateVectorOfSortedTables(&resources);
        gear::assets::ShaderBuilder shaderBuilder(fbb);
        shaderBuilder.add_vertexText(vertTextOffset);
        shaderBuilder.add_fragmentText(fragTextOffset);
        shaderBuilder.add_isBinary(true);
        shaderBuilder.add_resources(fbbResources);
        shader = shaderBuilder.Finish();
    }
    else
    {
        auto vertTextOffset = gear::assets::CreateShaderTextDirect(fbb, vertexText.c_str(), nullptr);
        auto fragTextOffset = gear::assets::CreateShaderTextDirect(fbb, fragmentText.c_str(), nullptr);
        gear::assets::ShaderBuilder shaderBuilder(fbb);
        shaderBuilder.add_vertexText(vertTextOffset);
        shaderBuilder.add_fragmentText(fragTextOffset);
        shaderBuilder.add_isBinary(false);
        shader = shaderBuilder.Finish();
    }

    std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
    entries.push_back(gear::assets::CreateAssetEntry(fbb, flatbuffers::HashFnv1<uint64_t>(name.c_str()), gear::assets::Asset::Shader, shader.Union()));
    auto assetVec = fbb.CreateVectorOfSortedTables(&entries);
    auto bundle = gear::assets::CreateBundle(fbb, assetVec);
    fbb.Finish(bundle);

    {
        auto buf = fbb.GetBufferPointer();
        auto bufSize = fbb.GetSize();
        std::ofstream ofs(outFileName, std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }

    {
        //write depfile
        std::ofstream ofs(outFileName + ".d");
        ofs << outFileName << ": " << vertexPath << " " << fragmentPath;
    }

    return 0;
}