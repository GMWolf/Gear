//
// Created by felix on 05/03/2020.
//

#ifndef GEAR_ALLOCATORS_H
#define GEAR_ALLOCATORS_H

#include <cstddef>

namespace gear {

    struct Block {
        std::byte* ptr {nullptr}; // the aligned pointer
        size_t size {0}; // the size of the block

        operator bool() const{
            return ptr;
        }

    };

    class StackAllocator {
        Block block;
        std::byte* ptr;

    public:
        explicit StackAllocator(Block block);

        Block allocate(size_t size);

        void deallocate(Block b);

        void deallocateAll();
    };



}

#endif //GEAR_ALLOCATORS_H
