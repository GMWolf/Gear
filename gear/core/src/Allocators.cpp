//
// Created by felix on 05/03/2020.
//

#include <gear/Allocators.h>

gear::StackAllocator::StackAllocator(gear::Block block) : block(block), ptr(block.ptr) {
}

gear::Block gear::StackAllocator::allocate(size_t size) {
    if (ptr + size > block.ptr + block.size) {
        return {};
    }

    Block b {ptr, size};
    ptr += size;
    return b;
}

void gear::StackAllocator::deallocate(gear::Block b) {
    if (b.ptr + b.size == ptr) {
        ptr = b.ptr;
    }
}

void gear::StackAllocator::deallocateAll() {
    ptr = block.ptr;
}
