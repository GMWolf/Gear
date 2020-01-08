//
// Created by felix on 08/01/2020.
//

#include <gear/View.h>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 gear::View::matrix() {
    return glm::ortho(pos.x, pos.x + size.x, pos.y, pos.y + size.y);
}
