//
// Created by felix on 08/11/2020.
//

#ifndef GEAR_MESH_H
#define GEAR_MESH_H

#include <glad/glad.h>
#include <cstddef>
#include <unordered_map>
#include <gear/mesh_generated.h>
#include <vector>

namespace gear::g3d {

    struct MeshCache {

        MeshCache();

        struct Buffer{
            explicit Buffer(size_t size);
            ~Buffer();
            GLuint vbo;
            size_t offset;
        };

        struct Mesh {
            struct Primitive {
                uint32_t baseVertex;
                uint32_t indexOffset;
                uint32_t indexCount;
                const assets::Material* material;
            };
            std::vector<Primitive> primitives;
        };


        struct MeshletBuffer {
            Buffer positions {10 * 1024 * 1024};
            Buffer texcoord {10 * 1024 * 1024};
            Buffer normals {10 * 1024 * 1024};
            Buffer tangents {10 * 1024 * 1024};
            Buffer indices {10 * 1024 * 1024};
        } meshletBuffer;


        GLuint meshletVAO;

        struct MeshletPrimitive {
            std::vector<GLsizei> count;
            std::vector<const void*> indices;
            std::vector<GLint> baseVertex;
        };

        uint32_t meshletVertexOffset = 0;
        uint32_t meshletIndexOffset = 0;

        Buffer positions;
        Buffer texcoord;
        Buffer normals;
        Buffer tangents;
        Buffer indices;
        GLuint vao;
        size_t vertexOffset;
        size_t indexOffset;

        std::unordered_map<const assets::Mesh*, Mesh> meshes;
        std::unordered_map<const assets::MeshPrimitive*, MeshletPrimitive> meshletPrimitives;

        Mesh& get(const assets::Mesh* mesh);
        MeshletPrimitive& getMeshletPrimitive(const assets::MeshPrimitive* meshPrimitive);

    private:
        Mesh::Primitive addPrimitive(const assets::MeshPrimitive* prim);
        Mesh addMesh(const assets::Mesh* mesh);
        MeshletPrimitive addMeshlets(const assets::MeshPrimitive* meshPrimitive);
    };

}


#endif //GEAR_MESH_H
