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
                uint32_t offset;
                uint32_t first;
                uint32_t indexCount;
                const assets::Material* material;
            };
            std::vector<Primitive> primitives;
        };

        Buffer positions;
        Buffer texcoord;
        Buffer normals;
        Buffer tangents;
        Buffer indices;
        GLuint vao;
        size_t vertexOffset;
        size_t indexOffset;

        std::unordered_map<const assets::Mesh*, Mesh> meshes;

        Mesh& get(const assets::Mesh* mesh);

    private:
        Mesh::Primitive addPrimitive(const assets::MeshPrimitive* prim);
        Mesh addMesh(const assets::Mesh* mesh);
    };

}


#endif //GEAR_MESH_H
