//
// Created by felix on 09/11/2020.
//

#ifndef GEAR_COMPONENTS_H
#define GEAR_COMPONENTS_H

#include "Components.h"

namespace gear {

    namespace assets {
        class Mesh;
        class Shader;
        class Texture;
    }

    struct MeshInstance {
        const assets::Mesh* mesh;
        const assets::Shader* shader;
        const assets::Texture* texture;
    };

}

#endif //GEAR_COMPONENTS_H
