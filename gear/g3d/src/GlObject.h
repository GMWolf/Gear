//
// Created by felix on 07/11/2020.
//

#ifndef GEAR_GLOBJECT_H
#define GEAR_GLOBJECT_H

#include <glad/glad.h>

namespace gear::g3d {

    template<typename T>
    concept Releasable = requires (T t) {
        t.release();
    };

    template<typename DERIVED>
    struct GlObject {
        GlObject() : id(0){};
        explicit GlObject(GLuint id) : id(id){};
        GlObject(const GlObject &) = delete;
        GlObject &operator=(const GlObject &) = delete;

        ~GlObject() requires Releasable<DERIVED> {
            static_cast<DERIVED*>(this)->release();
        }

        GlObject(GlObject&& other) noexcept : id(other.id) {
            other.id = 0;
        }

        GlObject& operator=(GlObject&& other) noexcept requires Releasable<DERIVED> {
            if (this != &other) {
                static_cast<DERIVED*>(this)->release();
                id = other.id;
                other.id = 0;
            }
            return *this;
        }

        GLuint id;
    };

}
#endif //GEAR_GLOBJECT_H
