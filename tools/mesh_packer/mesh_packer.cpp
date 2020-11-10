//
// Created by felix on 08/11/2020.
//

#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>
#include <iostream>
#include <gear/mesh_generated.h>
#include <gear/assets_generated.h>
#include <flatbuffers/hash.h>
#include <filesystem>

namespace fs = std::filesystem;

template<class T>
T readComponent(const void* ptr, int componentType) {
    switch(componentType) {
        case TINYGLTF_COMPONENT_TYPE_BYTE:
            return *static_cast<const int8_t*>(ptr);
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
            return *static_cast<const uint8_t*>(ptr);
        case TINYGLTF_COMPONENT_TYPE_SHORT:
            return *static_cast<const int16_t*>(ptr);
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
            return *static_cast<const uint16_t*>(ptr);
        case TINYGLTF_COMPONENT_TYPE_INT:
            return *static_cast<const int32_t*>(ptr);
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
            return *static_cast<const uint32_t*>(ptr);
        case TINYGLTF_COMPONENT_TYPE_FLOAT:
            return *static_cast<const float*>(ptr);
        case TINYGLTF_COMPONENT_TYPE_DOUBLE:
            return *static_cast<const double*>(ptr);
    }

    throw std::runtime_error("unknown type");
}

template<class T>
T access(const tinygltf::Model& model, const tinygltf::Accessor& accessor, int element, int component) {
    auto& bufferView = model.bufferViews[accessor.bufferView];
    auto& buffer = model.buffers[bufferView.buffer];
    const uint8_t* basePtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
    size_t stride = bufferView.byteStride ? bufferView.byteStride : tinygltf::GetComponentSizeInBytes(accessor.componentType) * tinygltf::GetNumComponentsInType(accessor.type);
    const uint8_t* elementPtr = basePtr + element * stride;
    const uint8_t* componentPtr = elementPtr + component * tinygltf::GetComponentSizeInBytes(accessor.componentType);
    return readComponent<T>(componentPtr, accessor.componentType);
}

template<class T>
std::vector<std::uint8_t> accessorRead(const tinygltf::Model& model, const tinygltf::Accessor& accessor) {

    int numComponents = tinygltf::GetNumComponentsInType(accessor.type);
    std::vector<std::uint8_t> vec(accessor.count * numComponents * sizeof(T));

    for(int element = 0; element < accessor.count; element++) {
        for(int comp = 0; comp < numComponents; comp++) {
            int index = element * numComponents + comp;

            reinterpret_cast<T*>(vec.data())[index] = access<T>(model, accessor, element, comp);
        }
    }

    return vec;
}

std::string getImageUri(const tinygltf::Model& model, int index) {
    auto& image = model.images[index];
    return image.uri;
}

int main(int argc, char* argv[]) {

    auto sourceFile = argv[1];
    auto destFile = argv[2];

    tinygltf::TinyGLTF loader;
    std::string gltfErr, gltfWarn;

    tinygltf::Model model;

    bool res = loader.LoadASCIIFromFile(&model, &gltfErr, &gltfWarn, sourceFile);
    if (!gltfWarn.empty()) {
        std::cerr << gltfWarn << "\n";
    }
    if (!gltfErr.empty()) {
        std::cerr << gltfErr << "\n";
        return 1;
    }

    if (!res) {
        std::cerr << "Failed to load gltf " << sourceFile << "\n";
        return 1;
    }

    flatbuffers::FlatBufferBuilder fbb(2048);

    std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> entries;
    std::vector<flatbuffers::Offset<gear::assets::Ref>> references;

    std::vector<flatbuffers::Offset<gear::assets::Ref>> textureReferences;
    std::vector<flatbuffers::Offset<gear::assets::Ref>> materialReferences;

    for(auto& texture : model.textures) {
        auto uri = getImageUri(model, texture.source);
        auto nameHash = flatbuffers::HashFnv1<uint64_t>(fs::path(uri).stem().c_str());
        auto ref = gear::assets::CreateRef(fbb, (uint8_t)gear::assets::Asset::Texture, nameHash);
        references.push_back(ref);
        textureReferences.push_back(ref);
    }

    for(auto& material : model.materials) {
        auto albedoRef = textureReferences[material.pbrMetallicRoughness.baseColorTexture.index];
        auto metallicRoughnessRef = textureReferences[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
        auto normalRef = textureReferences[material.normalTexture.index];
        auto aoRef = textureReferences[material.occlusionTexture.index];
        auto materialFb = gear::assets::CreateMaterial(fbb, albedoRef, normalRef, metallicRoughnessRef, aoRef);
        auto nameHash = flatbuffers::HashFnv1<uint64_t>(material.name.c_str());
        entries.push_back(gear::assets::CreateAssetEntry(fbb, nameHash, gear::assets::Asset::Material, materialFb.Union()));
        auto ref = gear::assets::CreateRef(fbb, (uint8_t)gear::assets::Asset::Material, nameHash);
        references.push_back(ref);
        materialReferences.push_back(ref);
    }

    for(auto& mesh : model.meshes) {
        std::vector<flatbuffers::Offset<gear::assets::MeshPrimitive>> primitives;
        for(auto& primitive : mesh.primitives) {
            auto& indexAccessor = model.accessors[primitive.indices];
            std::vector<uint8_t> indices = accessorRead<uint32_t>(model, model.accessors[primitive.indices]);
            std::vector<uint8_t> positions = accessorRead<float>(model, model.accessors[primitive.attributes["POSITION"]]);
            std::vector<uint8_t> normals = accessorRead<float>(model, model.accessors[primitive.attributes["NORMAL"]]);
            std::vector<uint8_t> texcoords = accessorRead<float>(model, model.accessors[primitive.attributes["TEXCOORD_0"]]);
            std::vector<uint8_t> tangents = accessorRead<float>(model, model.accessors[primitive.attributes["TANGENT"]]);
            auto matRef = materialReferences[primitive.material];
            primitives.push_back(gear::assets::CreateMeshPrimitiveDirect(fbb, model.accessors[primitive.indices].count,
                                                                         model.accessors[primitive.attributes["POSITION"]].count,
                                                                         &indices, &positions, &texcoords, &normals, &tangents,
                                                                         matRef));
        }

        auto meshBin = gear::assets::CreateMeshDirect(fbb, &primitives);
        entries.push_back(gear::assets::CreateAssetEntry(fbb, flatbuffers::HashFnv1<uint64_t>(mesh.name.c_str()), gear::assets::Asset::Mesh, meshBin.Union()));
    }


    auto assetVec = fbb.CreateVectorOfSortedTables(&entries);
    auto refVec = fbb.CreateVector(references);
    auto bundle = gear::assets::CreateBundle(fbb, assetVec, 0, refVec);
    fbb.Finish(bundle);
    {
        auto buf = fbb.GetBufferPointer();
        auto bufSize = fbb.GetSize();
        std::ofstream ofs(destFile, std::ios::out | std::ios::binary);
        ofs.write((char*)buf, bufSize);
    }
}