//
// Created by felix on 08/01/2020.
//

#ifndef GEAR_COLLISIONDETECTION_H
#define GEAR_COLLISIONDETECTION_H

#include <glm/vec2.hpp>
#include "CollisionShape.h"

namespace gear {

    bool collide(CollisionShape a, glm::vec2 pa, CollisionShape b, glm::vec2 pb);

    bool collide(Circle a, glm::vec2 pa, Circle b, glm::vec2 pb);

    bool collide(Rectangle a, glm::vec2 pa, Rectangle b, glm::vec2 pb);

    bool collide(Circle a, glm::vec2 pa, Rectangle b, glm::vec2 pb);

    bool collide(Rectangle a, glm::vec2 pa, Circle b, glm::vec2 pb);

}

#endif //GEAR_COLLISIONDETECTION_H
