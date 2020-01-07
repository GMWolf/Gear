//
// Created by felix on 08/01/2020.
//

#ifndef GEAR_COLLISIONSHAPE_H
#define GEAR_COLLISIONSHAPE_H

#include <glm/vec2.hpp>
#include <variant>

namespace gear {

    struct Rectangle {
        glm::vec2 min {0,0};
        glm::vec2 max {1,1};
    };

    struct Circle {
        glm::vec2 center;
        float radius;
    };

    using CollisionShape = std::variant<Rectangle, Circle>;

}
#endif //GEAR_COLLISIONSHAPE_H
