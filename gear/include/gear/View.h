//
// Created by felix on 08/01/2020.
//

#ifndef GEAR_CAMERA_H
#define GEAR_CAMERA_H

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
namespace gear {

    struct View {
        glm::vec2 pos;
        glm::vec2 size;

        glm::mat4 matrix();
    };



}

#endif //GEAR_CAMERA_H
