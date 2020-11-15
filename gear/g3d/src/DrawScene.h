//
// Created by felix on 15/11/2020.
//

#ifndef GEAR_DRAWSCENE_H
#define GEAR_DRAWSCENE_H

#include "Buffer.h"
#include <glm/glm.hpp>
#include <gear/Transform.h>

namespace gear::g3d {

    struct CameraBufferData {
        Transform3 cameraTransform;
        glm::mat4 projection;
    };


    struct BatchBuffers {
        g3d::UniformBuffer<CameraBufferData, 3> cameraBuffer;
        g3d::RingBuffer transformBuffer {sizeof(Transform3) * 1024 * 4 * 3};
    };



}

#endif //GEAR_DRAWSCENE_H
