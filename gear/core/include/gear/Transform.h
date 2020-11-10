//
// Created by felix on 14/01/2020.
//

#ifndef GEAR_TRANSFORM_H
#define GEAR_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace gear {

    struct Transform {
        glm::vec2 pos;
    };

    struct Transform3 {
        glm::vec3 position {}; float padding;
        glm::quat orientation {};


        glm::mat4x3 matrix() const;

        Transform3 apply(const Transform3& transform3) const;
        Transform3 inverse() const;
    };

}

#endif //GEAR_TRANSFORM_H
