//
// Created by felix on 11/06/2020.
//

#include <gear/Transform.h>
#include <flatbuffers/flexbuffers.h>


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

}