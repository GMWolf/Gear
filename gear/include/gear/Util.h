//
// Created by felix on 27/03/2020.
//

#ifndef GEAR_UTIL_H
#define GEAR_UTIL_H

#include <gear/ECS/Core.h>

namespace gear {

    template<class T>
    class ArrayRange {
        T* _begin {nullptr};
        T* _end {nullptr};

    public:
        T* begin() {
            return _begin;
        }

        T* end() {
            return _end;
        }

        ArrayRange(T *begin, T *end) : _begin(begin), _end(end) {};
        ArrayRange() : _begin(nullptr), _end(nullptr) {};
    };
}
#endif //GEAR_UTIL_H
