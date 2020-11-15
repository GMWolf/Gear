//
// Created by felix on 31/10/2020.
//

#ifndef GEAR_G3D_H
#define GEAR_G3D_H

#include "Components.h"
#include <gear/gapi.h>
#include <memory>
#include <glm/vec4.hpp>


namespace gear {

    const Gapi* g3dGetGapi();

    namespace g3d {
        struct TextureCache;
        struct ShaderCache;
        struct MeshCache;
        struct BatchBuffers;
    }

    namespace ecs {
        class Registry;
        class CommandBuffer;
    }

    namespace assets {
        struct Texture;
        struct Shader;
        struct Mesh;
    }

    struct G3DInstance {
        G3DInstance();
        ~G3DInstance();
        std::unique_ptr<g3d::TextureCache> textureCache;
        std::unique_ptr<g3d::ShaderCache> shaderCache;
        std::unique_ptr<g3d::MeshCache> meshCache;
        std::unique_ptr<g3d::BatchBuffers> batchBuffers;

        void debugTexture(const assets::Texture* texture, const assets::Shader* shader);
        void debugMesh(const assets::Mesh* mesh, const assets::Texture* texture, const assets::Shader* shader);
        void clearBuffer(glm::vec4 color, float depth);

        void renderScene(ecs::Registry& registry);
    };

}

#endif //GEAR_G3D_H
