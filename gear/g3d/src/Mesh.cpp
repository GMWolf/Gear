//
// Created by felix on 08/11/2020.
//

#include "Mesh.h"

gear::g3d::MeshCache::MeshCache() :
    positions(1024 * 1024),
    texcoord(1024 * 1024),
    normals(1024 * 1024),
    indices(1024 * 1024),
    vertexOffset(0),
    indexOffset(0),
    vao(0){

    glCreateVertexArrays(1, &vao);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0); //pos
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 0); //norm
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, 0); //texcoord

    glVertexArrayVertexBuffer(vao, 0, positions.vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(vao, 1, normals.vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(vao, 2, texcoord.vbo, 0, 2 * sizeof(float));
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 1);
    glVertexArrayAttribBinding(vao, 2, 2);

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glEnableVertexArrayAttrib(vao, 2);

    glVertexArrayElementBuffer(vao, indices.vbo);
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
    glNamedBufferSubData(indices.vbo, indices.offset, prim->indices()->size(), prim->indices()->data());
    indices.offset += prim->indices()->size();

    Mesh::Primitive meshPrim{};
    meshPrim.indexCount = prim->indexCount();
    meshPrim.offset = vertexOffset;
    vertexOffset += prim->vertexCount();
    meshPrim.first = indexOffset;
    indexOffset += prim->indexCount();

    return meshPrim;
}

gear::g3d::MeshCache::Mesh gear::g3d::MeshCache::get(const assets::Mesh* mesh) {
    auto it = meshes.find(mesh);
    if(it == meshes.end()) {
        it = meshes.insert({mesh, addMesh(mesh)}).first;
    }
    return it->second;
}

gear::g3d::MeshCache::Buffer::Buffer(size_t size) : vbo(0){
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, size, nullptr,  GL_DYNAMIC_STORAGE_BIT);
    offset = 0;
}

gear::g3d::MeshCache::Buffer::~Buffer() {
    glDeleteBuffers(1, &vbo);
}
