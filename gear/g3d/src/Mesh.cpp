//
// Created by felix on 08/11/2020.
//

#include "Mesh.h"



gear::g3d::MeshCache::MeshCache() :
    positions(10 * 1024 * 1024),
    texcoord(10 * 1024 * 1024),
    normals(10 * 1024 * 1024),
    indices(10 * 1024 * 1024),
    tangents(10 * 1024 * 1024),
    vertexOffset(0),
    indexOffset(0),
    vao(0){

    {
        glCreateVertexArrays(1, &vao);

        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0); //positions
        glVertexArrayAttribFormat(vao, 1, 4, GL_INT_2_10_10_10_REV, GL_TRUE, 0); //normals
        glVertexArrayAttribFormat(vao, 2, 2, GL_UNSIGNED_SHORT, GL_TRUE, 0); //texcoord
        glVertexArrayAttribFormat(vao, 3, 4, GL_INT_2_10_10_10_REV, GL_TRUE, 0); //tangent

        glVertexArrayVertexBuffer(vao, 0, positions.vbo, 0, 3 * sizeof(float));
        glVertexArrayVertexBuffer(vao, 1, normals.vbo, 0, sizeof(uint32_t));
        glVertexArrayVertexBuffer(vao, 2, texcoord.vbo, 0, sizeof(uint32_t));
        glVertexArrayVertexBuffer(vao, 3, tangents.vbo, 0, sizeof(uint32_t));
        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribBinding(vao, 1, 1);
        glVertexArrayAttribBinding(vao, 2, 2);
        glVertexArrayAttribBinding(vao, 3, 3);

        glEnableVertexArrayAttrib(vao, 0);
        glEnableVertexArrayAttrib(vao, 1);
        glEnableVertexArrayAttrib(vao, 2);
        glEnableVertexArrayAttrib(vao, 3);

        glVertexArrayElementBuffer(vao, indices.vbo);
    }

    {
        glCreateVertexArrays(1, &meshletVAO);

        glVertexArrayAttribFormat(meshletVAO, 0, 3, GL_FLOAT, GL_FALSE, 0); //positions
        glVertexArrayAttribFormat(meshletVAO, 1, 4, GL_INT_2_10_10_10_REV, GL_TRUE, 0); //normals
        glVertexArrayAttribFormat(meshletVAO, 2, 2, GL_UNSIGNED_SHORT, GL_TRUE, 0); //texcoord
        glVertexArrayAttribFormat(meshletVAO, 3, 4, GL_INT_2_10_10_10_REV, GL_TRUE, 0); //tangent

        glVertexArrayVertexBuffer(meshletVAO, 0, meshletBuffer.positions.vbo, 0, 3 * sizeof(float));
        glVertexArrayVertexBuffer(meshletVAO, 1, meshletBuffer.normals.vbo, 0, sizeof(uint32_t));
        glVertexArrayVertexBuffer(meshletVAO, 2, meshletBuffer.texcoord.vbo, 0, sizeof(uint32_t));
        glVertexArrayVertexBuffer(meshletVAO, 3, meshletBuffer.tangents.vbo, 0, sizeof(uint32_t));
        glVertexArrayAttribBinding(meshletVAO, 0, 0);
        glVertexArrayAttribBinding(meshletVAO, 1, 1);
        glVertexArrayAttribBinding(meshletVAO, 2, 2);
        glVertexArrayAttribBinding(meshletVAO, 3, 3);

        glEnableVertexArrayAttrib(meshletVAO, 0);
        glEnableVertexArrayAttrib(meshletVAO, 1);
        glEnableVertexArrayAttrib(meshletVAO, 2);
        glEnableVertexArrayAttrib(meshletVAO, 3);

        glVertexArrayElementBuffer(meshletVAO, meshletBuffer.indices.vbo);

    }
}

gear::g3d::MeshCache::Mesh gear::g3d::MeshCache::addMesh(const gear::assets::Mesh* meshDef) {
    Mesh mesh;
    for(const auto& primDef : *meshDef->primitives()) {
        mesh.primitives.push_back(addPrimitive(primDef));
    }
    return mesh;
}

gear::g3d::MeshCache::Mesh::Primitive gear::g3d::MeshCache::addPrimitive(const gear::assets::MeshPrimitive* prim) {
    glNamedBufferSubData(positions.vbo, positions.offset, prim->position()->size(), prim->position()->data());
    positions.offset += prim->position()->size();
    glNamedBufferSubData(texcoord.vbo, texcoord.offset, prim->texcoord()->size(), prim->texcoord()->data());
    texcoord.offset += prim->texcoord()->size();
    glNamedBufferSubData(normals.vbo, normals.offset, prim->normals()->size(), prim->normals()->data());
    normals.offset += prim->normals()->size();
    glNamedBufferSubData(tangents.vbo, tangents.offset, prim->tangents()->size(), prim->tangents()->data());
    tangents.offset += prim->tangents()->size();
    glNamedBufferSubData(indices.vbo, indices.offset, prim->indices()->size(), prim->indices()->data());
    indices.offset += prim->indices()->size();

    Mesh::Primitive meshPrim{};
    meshPrim.indexCount = prim->indexCount();
    meshPrim.baseVertex = vertexOffset;
    vertexOffset += prim->vertexCount();
    meshPrim.indexOffset = indexOffset;
    indexOffset = indices.offset;
    meshPrim.material = (const assets::Material*)prim->material()->ptr();

    return meshPrim;
}

gear::g3d::MeshCache::Mesh& gear::g3d::MeshCache::get(const assets::Mesh* mesh) {
    auto it = meshes.find(mesh);
    if(it == meshes.end()) {
        it = meshes.insert({mesh, addMesh(mesh)}).first;
    }
    return it->second;
}



gear::g3d::MeshCache::MeshletPrimitive &gear::g3d::MeshCache::getMeshletPrimitive(
        const assets::MeshPrimitive *meshPrimitive) {
    auto it = meshletPrimitives.find(meshPrimitive);
    if(it == meshletPrimitives.end()) {
        it = meshletPrimitives.insert({meshPrimitive, addMeshlets(meshPrimitive)}).first;
    }
    return it->second;
}

gear::g3d::MeshCache::MeshletPrimitive gear::g3d::MeshCache::addMeshlets(const gear::assets::MeshPrimitive* meshPrim) {

    auto meshlets = meshPrim->meshlets();

    glNamedBufferSubData(meshletBuffer.positions.vbo, meshletBuffer.positions.offset, meshlets->positions()->size(), meshlets->positions()->data());
    meshletBuffer.positions.offset += meshlets->positions()->size();
    glNamedBufferSubData(meshletBuffer.texcoord.vbo, meshletBuffer.texcoord.offset, meshlets->texcoords()->size(), meshlets->texcoords()->data());
    meshletBuffer.texcoord.offset += meshlets->texcoords()->size();
    glNamedBufferSubData(meshletBuffer.normals.vbo, meshletBuffer.normals.offset, meshlets->normals()->size(), meshlets->normals()->data());
    meshletBuffer.normals.offset += meshlets->normals()->size();
    glNamedBufferSubData(meshletBuffer.tangents.vbo, meshletBuffer.tangents.offset, meshlets->tangents()->size(), meshlets->tangents()->data());
    meshletBuffer.tangents.offset += meshlets->tangents()->size();
    glNamedBufferSubData(meshletBuffer.indices.vbo, meshletBuffer.indices.offset, meshlets->indices()->size(), meshlets->indices()->data());
    meshletBuffer.indices.offset += meshlets->indices()->size();

    MeshletPrimitive mp{};
    for(auto c : *meshPrim->meshlets()->indexCounts()) {
        mp.count.push_back(c);
    }
    for(auto vo : *meshPrim->meshlets()->vertexOffsets()) {
        mp.baseVertex.push_back(vo + meshletVertexOffset);
    }
    meshletVertexOffset += meshlets->vertexCount();
    for(auto i : *meshPrim->meshlets()->indexOffsets()) {
        mp.indices.push_back((const void*) (uint64_t )(i + meshletIndexOffset));
    }
    meshletIndexOffset = meshletBuffer.indices.offset;

    return mp;
}

gear::g3d::MeshCache::Buffer::Buffer(size_t size) : vbo(0){
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, size, nullptr,  GL_DYNAMIC_STORAGE_BIT);
    offset = 0;
}

gear::g3d::MeshCache::Buffer::~Buffer() {
    glDeleteBuffers(1, &vbo);
}
