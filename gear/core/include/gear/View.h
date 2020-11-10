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

    struct ViewPort {
        glm::ivec2 pos;
        glm::ivec2 size;
    };

    struct Camera {
        ViewPort viewPort;
        float near, far;
        float fov;
    };

}

#endif //GEAR_CAMERA_H
