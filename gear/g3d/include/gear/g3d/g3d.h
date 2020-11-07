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
    }

    namespace assets {
        struct Texture;
    }

    struct G3DInstance {
        G3DInstance();
        std::unique_ptr<g3d::TextureCache> textureCache;
        void debugTexture(const assets::Texture* texture);

    };

}

#endif //GEAR_G3D_H
