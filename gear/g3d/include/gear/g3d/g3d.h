//
// Created by felix on 31/10/2020.
//

#ifndef GEAR_G3D_H
#define GEAR_G3D_H

#include <gear/gapi.h>
#include <memory>

namespace gear {
    const Gapi* g3dGetGapi();

    namespace g3d {
        struct TextureCache;
        struct ShaderCache;
        struct MeshCache;
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
        void debugTexture(const assets::Texture* texture, const assets::Shader* shader);
        void debugMesh(const assets::Mesh* mesh, const assets::Texture* texture, const assets::Shader* shader);
    };

}

#endif //GEAR_G3D_H
