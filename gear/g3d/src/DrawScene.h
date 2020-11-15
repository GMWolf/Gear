//
// Created by felix on 15/11/2020.
//

#ifndef GEAR_DRAWSCENE_H
#define GEAR_DRAWSCENE_H

#include "StreamBuffer.h"
#include <glm/glm.hpp>
#include <gear/Transform.h>

namespace gear::g3d {

    struct CameraBufferData {
        Transform3 cameraTransform;
        glm::mat4 projection;
    };


    struct MultiDrawCmdBuffer {
        static const size_t capacity = 2048;
        size_t drawCount;

        GLsizei indexCount[capacity];
        const void* indices[capacity];
        GLint baseVertex[capacity];

        g3d::StreamBuffer cameraBuffer {sizeof(CameraBufferData) * 3};
        g3d::StreamBuffer transformBuffer {sizeof(Transform3) * capacity * 3};
    };
}

#endif //GEAR_DRAWSCENE_H
