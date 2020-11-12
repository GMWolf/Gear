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
#include <meshoptimizer.h>
#include <algorithm>
#include <glm/glm.hpp>

namespace fs = std::filesystem;

template<class T>
T readComponent(const void* ptr, int componentType, bool normalize = false, bool sign = false) {
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
std::vector<T> accessorReadScalar(const tinygltf::Model& model, const tinygltf::Accessor& accessor) {

    std::vector<T> vec(accessor.count);
    for(int element = 0; element < accessor.count; element++) {
        vec[element] = access<T>(model, accessor, element, 0);
    }

    return vec;
}

template<class T>
std::vector<T> accessorReadVec(const tinygltf::Model& model, const tinygltf::Accessor& accessor) {

    int numComponents = tinygltf::GetNumComponentsInType(accessor.type);
    std::vector<T> vec(accessor.count);

    for(int element = 0; element < accessor.count; element++) {
        for(int comp = 0; comp < numComponents; comp++) {
            vec[element][comp] = access<typename T::value_type>(model, accessor, element, comp);
        }
    }
    return vec;
}

size_t getTypeSize(int type, int componentType) {
    return tinygltf::GetComponentSizeInBytes(componentType) * tinygltf::GetNumComponentsInType(type);
}

meshopt_Stream streamFromAccessor(const tinygltf::Model& model, const tinygltf::Accessor& accessor) {
    auto& bufferView = model.bufferViews[accessor.bufferView];
    auto& buffer = model.buffers[bufferView.buffer];

    meshopt_Stream stream{};
    stream.stride = bufferView.byteStride ? bufferView.byteStride : getTypeSize(accessor.type, accessor.componentType);
    stream.size = getTypeSize(accessor.type, accessor.componentType);
    stream.data = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
    return stream;
}

std::string getImageUri(const tinygltf::Model& model, int index) {
    auto& image = model.images[index];
    return image.uri;
}

template<class T>
meshopt_Stream vectorMeshoptStream(std::vector<T>& v) {
    meshopt_Stream stream{};
    stream.data = v.data();
    stream.size = sizeof(T);
    stream.stride = sizeof(T);
    return stream;
}


struct packedVec3 {
        int x : 10;
        int y : 10;
        int z : 10;
        int w : 2;

        packedVec3() = default;

        explicit packedVec3(glm::vec3 v) {
            *this = v;
        }

        explicit packedVec3(glm::vec4 v) {
            *this = v;
        }


    packedVec3& operator=(const glm::vec3& v) {
            x = meshopt_quantizeSnorm(v.x, 10);
            y = meshopt_quantizeSnorm(v.y, 10);
            z = meshopt_quantizeSnorm(v.z, 10);
            return *this;
        }
    packedVec3& operator=(const glm::vec4& v) {
        x = meshopt_quantizeSnorm(v.x, 10);
        y = meshopt_quantizeSnorm(v.y, 10);
        z = meshopt_quantizeSnorm(v.z, 10);
        w = meshopt_quantizeSnorm(v.w, 2);
        return *this;
    }
};

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

            auto indices = accessorReadScalar<uint32_t>(model, model.accessors[primitive.indices]);
            auto positions = accessorReadVec<glm::vec3>(model, model.accessors[primitive.attributes["POSITION"]]);
            auto normals = accessorReadVec<glm::vec3>(model, model.accessors[primitive.attributes["NORMAL"]]);
            auto texcoords = accessorReadVec<glm::vec2>(model, model.accessors[primitive.attributes["TEXCOORD_0"]]);
            auto tangents = accessorReadVec<glm::vec4>(model, model.accessors[primitive.attributes["TANGENT"]]);

            meshopt_Stream streams[] {
                    vectorMeshoptStream(positions),
                    vectorMeshoptStream(normals),
                    vectorMeshoptStream(texcoords),
                    vectorMeshoptStream(tangents),
            };

            auto inputVertexCount = positions.size();
            std::vector<uint32_t> remap(indices.size());
            size_t remapVertexCount = meshopt_generateVertexRemapMulti(remap.data(), indices.data(), indices.size(), inputVertexCount, streams, 4);
            meshopt_remapIndexBuffer(indices.data(), indices.data(), indices.size(), remap.data());
            meshopt_remapVertexBuffer(positions.data(), positions.data(), inputVertexCount, sizeof(glm::vec3), remap.data());
            positions.resize(remapVertexCount);
            meshopt_remapVertexBuffer(normals.data(), normals.data(), inputVertexCount, sizeof(glm::vec3), remap.data());
            normals.resize(remapVertexCount);
            meshopt_remapVertexBuffer(texcoords.data(), texcoords.data(), inputVertexCount, sizeof(glm::vec2), remap.data());
            texcoords.resize(remapVertexCount);
            meshopt_remapVertexBuffer(tangents.data(), tangents.data(), inputVertexCount, sizeof(glm::vec4), remap.data());
            tangents.resize(remapVertexCount);

            std::vector<packedVec3> packedNormals(normals.size());
            std::transform(normals.cbegin(), normals.cend(), packedNormals.begin(), [](glm::vec3 n) {
                return packedVec3(n);
            });

            std::vector<uint32_t> packedTexcoods(texcoords.size());
            std::transform(texcoords.cbegin(), texcoords.cend(), packedTexcoods.begin(), [](glm::vec2 t){
                return glm::packUnorm2x16(t);
            });

            std::vector<packedVec3> packedTangents(tangents.size());
            std::transform(tangents.cbegin(), tangents.cend(), packedTangents.begin(), [](glm::vec4 t) {
                return packedVec3(t);
            });

            auto indexBuffer = fbb.CreateVector((uint8_t*)indices.data(), indices.size() * sizeof(uint32_t));
            auto positionBuffer = fbb.CreateVector((uint8_t*)positions.data(), positions.size() * sizeof(glm::vec3));
            auto normalBuffer = fbb.CreateVector((uint8_t*)packedNormals.data(), packedNormals.size() * sizeof(uint32_t));
            auto texcoordBuffer = fbb.CreateVector((uint8_t*)packedTexcoods.data(), packedTexcoods.size() * sizeof(uint32_t));
            auto tangentBuffer = fbb.CreateVector((uint8_t*)packedTangents.data(), packedTangents.size() * sizeof(uint32_t));

            auto matRef = materialReferences[primitive.material];
            primitives.push_back(gear::assets::CreateMeshPrimitive(fbb, model.accessors[primitive.indices].count, remapVertexCount,
                                                                         indexBuffer, positionBuffer, texcoordBuffer, normalBuffer, tangentBuffer,
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