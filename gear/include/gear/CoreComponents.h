//
// Created by felix on 14/01/2020.
//

#ifndef GEAR_CORECOMPONENTS_H
#define GEAR_CORECOMPONENTS_H

#include <glm/vec2.hpp>
#include "ECS/ECS.h"

namespace gear {

    struct Transform {
        glm::vec2 pos;
    };

    struct WorldTransform {
        glm::vec2 pos;
    };

    struct Parent {
        ecs::EntityId entity;
    };

    struct ParentInternal {
        int depth;
    };

}

#endif //GEAR_CORECOMPONENTS_H
