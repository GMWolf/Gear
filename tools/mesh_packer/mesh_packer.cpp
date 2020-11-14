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


std::vector<glm::vec4> &
remapTangentAverages(std::vector<glm::vec4> &tangents, unsigned long inputVertexCount, const std::vector<uint32_t> &remap,
                          size_t remapVertexCount) {
    std::vector<glm::vec4> outTangent(remapVertexCount, glm::vec4(0, 0, 0, 0));
    for(int i = 0; i < inputVertexCount; i++) {
        outTangent[remap[i]] += tangents[i];
    }
    for(int i = 0; i < remapVertexCount; i++) {
        glm::vec3 vec(outTangent[i]);
        vec = glm::normalize(vec);
        tangents[i] = glm::vec4(vec, 1);
    }
    tangents.resize(remapVertexCount);
    return tangents;
}



struct MeshletBuffer {
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec4> tangents;

    std::vector<uint32_t> vertexOffsets;
    std::vector<uint32_t> indexOffsets;
    std::vector<uint32_t> triangleCounts;

    std::vector<meshopt_Bounds> bounds;
};

struct PackedMeshletBuffer {
    std::vector<uint8_t> indices;
    std::vector<glm::vec3> positions;
    std::vector<packedVec3> normals;
    std::vector<uint32_t> texcoords;
    std::vector<packedVec3> tangents;

    std::vector<int32_t> vertexOffsets;
    std::vector<uint32_t> indexOffsets;
    std::vector<int32_t> indexCounts;

    std::vector<meshopt_Bounds> bounds;
};

struct MeshData {
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec4> tangents;
};

MeshletBuffer createMeshlets(const MeshData& meshData) {
    size_t maxVertices = 64;
    size_t maxTriangles = 126;

    std::vector<meshopt_Meshlet> meshlets(meshopt_buildMeshletsBound(meshData.indices.size(), maxVertices, maxTriangles));
    auto meshletCount = meshopt_buildMeshlets(meshlets.data(), meshData.indices.data(), meshData.indices.size(), meshData.positions.size(), maxVertices, maxTriangles);
    meshlets.resize(meshletCount);

    MeshletBuffer meshletBuffer;
    meshletBuffer.indices.reserve(meshletCount * maxTriangles * 3);
    meshletBuffer.positions.reserve(meshletCount * maxVertices);
    meshletBuffer.normals.reserve(meshletCount * maxVertices);
    meshletBuffer.texcoords.reserve(meshletCount * maxVertices);
    meshletBuffer.tangents.reserve(meshletCount * maxVertices);
    meshletBuffer.vertexOffsets.reserve(meshletCount);
    meshletBuffer.indexOffsets.reserve(meshletCount);
    meshletBuffer.triangleCounts.reserve(meshletCount);
    meshletBuffer.bounds.reserve(meshletCount);

    for(auto& meshlet : meshlets) {
        meshletBuffer.triangleCounts.push_back(meshlet.triangle_count);
        meshletBuffer.indexOffsets.push_back(meshletBuffer.indices.size());
        meshletBuffer.vertexOffsets.push_back(meshletBuffer.positions.size());

        //write micro vertex buffer
        for(int vert = 0; vert < meshlet.vertex_count; vert++) {
            auto index = meshlet.vertices[vert];
            meshletBuffer.positions.push_back(meshData.positions[index]);
            meshletBuffer.normals.push_back(meshData.normals[index]);
            meshletBuffer.texcoords.push_back(meshData.texcoords[index]);
            meshletBuffer.tangents.push_back(meshData.tangents[index]);
        }

        //write indices
        for(int tri = 0; tri < meshlet.triangle_count; tri++) {
            for(int v = 0; v < 3; v++) {
                auto index = meshlet.indices[tri][v];
                meshletBuffer.indices.push_back(index);
            }
        }

        //compute bounds
        meshletBuffer.bounds.push_back(meshopt_computeMeshletBounds(&meshlet, &meshData.positions[0].x, meshData.positions.size(), sizeof(glm::vec3)));
    }

    return meshletBuffer;
}


PackedMeshletBuffer packMeshlets(const MeshletBuffer& meshletBuffer) {

    PackedMeshletBuffer packedBuffer;

    packedBuffer.indices.resize(meshletBuffer.indices.size());
    std::transform(meshletBuffer.indices.begin(), meshletBuffer.indices.end(), packedBuffer.indices.begin(), [](uint32_t i) {
        return (uint8_t)i;
    });

    packedBuffer.positions.resize(meshletBuffer.positions.size());
    std::transform(meshletBuffer.positions.begin(), meshletBuffer.positions.end(), packedBuffer.positions.begin(), [](glm::vec3 pos) {
        return pos;
    });

    packedBuffer.normals.resize(meshletBuffer.normals.size());
    std::transform(meshletBuffer.normals.cbegin(), meshletBuffer.normals.cend(), packedBuffer.normals.begin(), [](glm::vec3 n) {
        return packedVec3(n);
    });

    packedBuffer.texcoords.resize(meshletBuffer.texcoords.size());
    std::transform(meshletBuffer.texcoords.cbegin(), meshletBuffer.texcoords.cend(), packedBuffer.texcoords.begin(), [](glm::vec2 t){
        return glm::packUnorm2x16(t);
    });

    packedBuffer.tangents.resize(meshletBuffer.tangents.size());
    std::transform(meshletBuffer.tangents.cbegin(), meshletBuffer.tangents.cend(), packedBuffer.tangents.begin(), [](glm::vec4 t) {
        return packedVec3(t);
    });

    packedBuffer.indexCounts.resize(meshletBuffer.triangleCounts.size());
    std::transform(meshletBuffer.triangleCounts.cbegin(), meshletBuffer.triangleCounts.cend(), packedBuffer.indexCounts.begin(), [](uint32_t i) {
        return i * 3;
    });

    packedBuffer.indexOffsets.resize(meshletBuffer.indexOffsets.size());
    std::transform(meshletBuffer.indexOffsets.cbegin(), meshletBuffer.indexOffsets.cend(), packedBuffer.indexOffsets.begin(), [](uint32_t i) {
        return i;
    });

    packedBuffer.vertexOffsets.resize(meshletBuffer.vertexOffsets.size());
    std::transform(meshletBuffer.vertexOffsets.cbegin(), meshletBuffer.vertexOffsets.cend(), packedBuffer.vertexOffsets.begin(), [](uint32_t i) {
        return i;
    });

    packedBuffer.bounds = meshletBuffer.bounds;

    return packedBuffer;
}


template<class T>
flatbuffers::Offset<flatbuffers::Vector<uint8_t>> writeAsBytes(flatbuffers::FlatBufferBuilder& fbb, const std::vector<T>& vec) {
    return fbb.CreateVector((uint8_t*)vec.data(), vec.size() * sizeof(T));
}

static void set(gear::assets::fvec3& fv, const glm::vec3& v) {
    fv.mutate_x(v.x);
    fv.mutate_y(v.y);
    fv.mutate_z(v.z);
}
static void set(gear::assets::fvec3& fv, float v[3]) {
    fv.mutate_x(v[0]);
    fv.mutate_y(v[1]);
    fv.mutate_z(v[2]);
}

flatbuffers::Offset<gear::assets::MeshletBuffer> writeMeshletBuffer(flatbuffers::FlatBufferBuilder& fbb, const PackedMeshletBuffer& meshletBuffer) {
    auto indexBuffer = writeAsBytes(fbb, meshletBuffer.indices);
    auto positionBuffer = writeAsBytes(fbb, meshletBuffer.positions);
    auto normalBuffer = writeAsBytes(fbb, meshletBuffer.normals);
    auto texcoordBuffer = writeAsBytes(fbb, meshletBuffer.texcoords);
    auto tangentBuffer = writeAsBytes(fbb, meshletBuffer.tangents);

    auto indexOffsetsBuffer = fbb.CreateVector(meshletBuffer.indexOffsets);
    auto vertexOffsetsBuffer = fbb.CreateVector(meshletBuffer.vertexOffsets);
    auto indexCountsBuffer = fbb.CreateVector(meshletBuffer.indexCounts);

    std::vector<gear::assets::MeshletBounds> boundsVec;
    boundsVec.reserve(meshletBuffer.bounds.size());
    for(auto bounds : meshletBuffer.bounds) {
        gear::assets::MeshletBounds meshletBounds;
        set(meshletBounds.mutable_center(), bounds.center);
        meshletBounds.mutate_radius(bounds.radius);
        set(meshletBounds.mutable_coneApex(), bounds.cone_apex);
        set(meshletBounds.mutable_coneAxis(), bounds.cone_axis);
        meshletBounds.mutate_coneCutoff(bounds.cone_cutoff);
        boundsVec.push_back(meshletBounds);
    }
    auto boundsBuffer = fbb.CreateVectorOfStructs(boundsVec);


    gear::assets::MeshletBufferBuilder builder(fbb);
    builder.add_indices(indexBuffer);
    builder.add_positions(positionBuffer);
    builder.add_normals(normalBuffer);
    builder.add_texcoords(texcoordBuffer);
    builder.add_tangents(tangentBuffer);
    builder.add_indexOffsets(indexOffsetsBuffer);
    builder.add_vertexOffsets(vertexOffsetsBuffer);
    builder.add_indexCounts(indexCountsBuffer);
    builder.add_vertexCount(meshletBuffer.positions.size());
    builder.add_bounds(boundsBuffer);
    return builder.Finish();
}


uint32_t remapMesh(MeshData &mesh) {
    std::vector<uint32_t> remap(mesh.indices.size());

    meshopt_Stream streams[] {
            vectorMeshoptStream(mesh.positions),
            vectorMeshoptStream(mesh.normals),
            vectorMeshoptStream(mesh.texcoords),
            vectorMeshoptStream(mesh.tangents),
    };

    auto inputVertexCount = mesh.positions.size();
    uint32_t remapVertexCount = meshopt_generateVertexRemapMulti(remap.data(), mesh.indices.data(), mesh.indices.size(), inputVertexCount, streams, 3);
    meshopt_remapIndexBuffer(mesh.indices.data(), mesh.indices.data(), mesh.indices.size(), remap.data());
    meshopt_remapVertexBuffer(mesh.positions.data(), mesh.positions.data(), inputVertexCount, sizeof(glm::vec3), remap.data());
    mesh.positions.resize(remapVertexCount);
    meshopt_remapVertexBuffer(mesh.normals.data(), mesh.normals.data(), inputVertexCount, sizeof(glm::vec3), remap.data());
    mesh.normals.resize(remapVertexCount);
    meshopt_remapVertexBuffer(mesh.texcoords.data(), mesh.texcoords.data(), inputVertexCount, sizeof(glm::vec2), remap.data());
    mesh.texcoords.resize(remapVertexCount);
    mesh.tangents = remapTangentAverages(mesh.tangents, inputVertexCount, remap, remapVertexCount);
    return remapVertexCount;
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

            MeshData meshData{};
            meshData.indices = accessorReadScalar<uint32_t>(model, model.accessors[primitive.indices]);
            meshData.positions = accessorReadVec<glm::vec3>(model, model.accessors[primitive.attributes["POSITION"]]);
            meshData.normals = accessorReadVec<glm::vec3>(model, model.accessors[primitive.attributes["NORMAL"]]);
            meshData.texcoords = accessorReadVec<glm::vec2>(model, model.accessors[primitive.attributes["TEXCOORD_0"]]);
            meshData.tangents = accessorReadVec<glm::vec4>(model, model.accessors[primitive.attributes["TANGENT"]]);

            size_t remapVertexCount = remapMesh(meshData);

            meshopt_optimizeVertexCache(meshData.indices.data(), meshData.indices.data(), meshData.indices.size(), remapVertexCount);


            std::vector<uint16_t> packedIndices(meshData.indices.size());
            std::transform(meshData.indices.begin(), meshData.indices.end(), packedIndices.begin(), [](uint32_t i) {
                return (uint16_t)i;
            });

            std::vector<packedVec3> packedNormals(meshData.normals.size());
            std::transform(meshData.normals.cbegin(), meshData.normals.cend(), packedNormals.begin(), [](glm::vec3 n) {
                return packedVec3(n);
            });

            std::vector<uint32_t> packedTexcoods(meshData.texcoords.size());
            std::transform(meshData.texcoords.cbegin(), meshData.texcoords.cend(), packedTexcoods.begin(), [](glm::vec2 t){
                return glm::packUnorm2x16(t);
            });

            std::vector<packedVec3> packedTangents(meshData.tangents.size());
            std::transform(meshData.tangents.cbegin(), meshData.tangents.cend(), packedTangents.begin(), [](glm::vec4 t) {
                return packedVec3(t);
            });



            auto meshletBuffer = createMeshlets(meshData);
            auto packedMeshletBuffer = packMeshlets(meshletBuffer);
            auto meshletBufferOut = writeMeshletBuffer(fbb, packedMeshletBuffer);


            auto indexBuffer = fbb.CreateVector((uint8_t*)packedIndices.data(), packedIndices.size() * sizeof(uint16_t));
            auto positionBuffer = fbb.CreateVector((uint8_t*)meshData.positions.data(), meshData.positions.size() * sizeof(glm::vec3));
            auto normalBuffer = fbb.CreateVector((uint8_t*)packedNormals.data(), packedNormals.size() * sizeof(uint32_t));
            auto texcoordBuffer = fbb.CreateVector((uint8_t*)packedTexcoods.data(), packedTexcoods.size() * sizeof(uint32_t));
            auto tangentBuffer = fbb.CreateVector((uint8_t*)packedTangents.data(), packedTangents.size() * sizeof(uint32_t));

            auto matRef = materialReferences[primitive.material];
            primitives.push_back(gear::assets::CreateMeshPrimitive(fbb, model.accessors[primitive.indices].count, remapVertexCount,
                                                                         indexBuffer, positionBuffer, texcoordBuffer, normalBuffer, tangentBuffer,
                                                                         matRef, meshletBufferOut));
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