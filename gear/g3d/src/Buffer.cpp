//
// Created by felix on 14/11/2020.
//

#include "Buffer.h"
#include <stdexcept>
#include <iostream>

gear::g3d::Buffer::Buffer(size_t size, GLbitfield flags) : size(size) {
    glCreateBuffers(1, &id);
    glNamedBufferStorage(id, size, nullptr, flags);
}

void gear::g3d::Buffer::release() {
    glDeleteBuffers(1, &id);
    id = 0;
}


gear::g3d::PersistentlyMappedBuffer::PersistentlyMappedBuffer(size_t size)
    : Buffer(size, mapFlags), map(glMapNamedBufferRange(id, 0, size, mapFlags)) {
}

gear::g3d::RingBuffer::RingBuffer(size_t size) : PersistentlyMappedBuffer(size) {

}

static void waitOnFence(GLsync sync) {
    bool waited = false;
    if (sync) {
        GLenum result = GL_TIMEOUT_EXPIRED;
        while (result == GL_TIMEOUT_EXPIRED) {
            result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 10);
            if (!waited && result != GL_ALREADY_SIGNALED) {
                std::cerr << "Waiting on buffer!\n";
                waited = true;
            }
        }
    }
}



gear::g3d::RingBuffer::Block gear::g3d::RingBuffer::allocateBlock(size_t numBytes, size_t align) {
    if (numBytes > size) {
        throw std::runtime_error("Requested block larger than buffer size");
    }

    //reset if not enough space in buffer
    if (writeHead + numBytes > size) {
        //wait on any fences after the start
        while(!fencedRegions.empty() && fencedRegions.front().start >= writeHead) {
            waitOnFence(fencedRegions.front().sync);
            glDeleteSync(fencedRegions.front().sync);
            fencedRegions.pop();
        }
        writeHead = 0;
    }

    //wait on any blocks that are in the way
    while(!fencedRegions.empty() && fencedRegions.front().start >= writeHead && fencedRegions.front().start < writeHead + numBytes) {
        waitOnFence(fencedRegions.front().sync);
        glDeleteSync(fencedRegions.front().sync);
        fencedRegions.pop();
    }

    FencedRegion& fr = fencedRegions.emplace();
    fr.sync = nullptr;
    fr.start = writeHead;
    fr.end = writeHead + numBytes;

    writeHead += numBytes;

    return Block{
            (uint8_t*)map + fr.start,fr.end - fr.start, &fr.sync
    };
}

void gear::g3d::RingBuffer::bind(GLenum target, GLuint binding, gear::g3d::RingBuffer::Block &block) {
    glBindBufferRange(target, binding, id, (uint8_t*)(block.ptr) - (uint8_t*)map, block.size);
}
