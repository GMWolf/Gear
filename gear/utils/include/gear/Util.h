//
// Created by felix on 27/03/2020.
//

#ifndef GEAR_UTIL_H
#define GEAR_UTIL_H

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


    inline size_t hashCombine(size_t a, size_t b) {
        return a ^ (b + 0x9e3779b9u + (a<<6u) + (a>>2u));
    }
}
#endif //GEAR_UTIL_H
