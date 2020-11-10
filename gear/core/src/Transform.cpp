//
// Created by felix on 11/06/2020.
//

#include <gear/Transform.h>
#include <flatbuffers/flexbuffers.h>
#include <glm/gtx/quaternion.hpp>


namespace gear {

    void serializeTransform(Transform *transform, flexbuffers::Builder& fbb) {
        fbb.FixedTypedVector(&transform->pos.x, 2);
    }

    void deserializeTransform(void *ptr, flexbuffers::Reference& ref) {
        auto v = ref.AsFixedTypedVector();
        auto t = new (ptr) Transform{};
        t->pos.x = v[0].AsFloat();
        t->pos.y = v[1].AsFloat();
    }

    glm::mat4x3 Transform3::matrix() const {
        auto r = glm::toMat3(orientation);
        glm::mat4x3 mat;
        mat[0] = r[0];
        mat[1] = r[1];
        mat[2] = r[2];
        mat[3] = position;
        return mat;
    }

    Transform3 Transform3::apply(const Transform3& base) const{
        Transform3 result;
        result.position = position + orientation * base.position;
        result.orientation = orientation * base.orientation;
        return result;
    }

    Transform3 Transform3::inverse() const {
        Transform3 result;
        result.position = glm::inverse(orientation) * -position;
        result.orientation = glm::inverse(orientation);
        return result;
    }
}