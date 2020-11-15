//
// Created by felix on 14/11/2020.
//

#include "StreamBuffer.h"

gear::g3d::StreamBuffer::StreamBuffer(size_t size) : size(size) {
    glCreateBuffers(1, &id);
    glNamedBufferStorage(id, size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    map = static_cast<uint8_t *>(glMapNamedBufferRange(id, 0, size,
                                                       GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));

    for(auto & fence : fences) {
        fence.sync = nullptr;
    }
}

void gear::g3d::StreamBuffer::release() {
    glDeleteBuffers(1, &id);
    id = 0;
    for(auto& fence : fences) {
        glDeleteSync(fence.sync);
    }
}

bool gear::g3d::StreamBuffer::clientWait() {
    if (writeFence == readFence) {
        return false;
    }
    auto& fence = fences[readFence];
    if (fence.sync) {
        GLenum result = GL_TIMEOUT_EXPIRED;
        while (result == GL_TIMEOUT_EXPIRED) {
            result = glClientWaitSync(fence.sync, GL_SYNC_FLUSH_COMMANDS_BIT, 10);
        }
        glDeleteSync(fence.sync);
        fence.sync = nullptr;
    }
    readFence++;
    if (readFence == maxFences) {
        readFence = 0;
    }
    return true;
}

size_t gear::g3d::StreamBuffer::available() const {
    if (writeFence == readFence) {
        return size;
    }
    const auto& fence = fences[readFence];
    if (head <= fence.start) {
        return fence.start - head;
    } else {
        return size - head;
    }
}

void gear::g3d::StreamBuffer::clientWaitUntilAvailable(size_t request) {
    while(available() < request) {
        clientWait();
    }
}

gear::g3d::StreamBuffer::BufferRegion* gear::g3d::StreamBuffer::finishWrite() {
    auto& fence = fences[writeFence++];
    if (writeFence == maxFences) {
        writeFence = 0;
    }
    if (writeFence == readFence) {
        clientWait();
    }
    fence.start = regionStart;
    fence.size = head - regionStart;
    regionStart = head;
    if (head == size) {
        regionStart = 0;
        head = 0;
    }
    return &fence;
}

void gear::g3d::StreamBuffer::clientWaitAndResetHead() {
    while(fences[readFence].start >= head) {
        clientWait();
    }
    head = 0;
}

void *gear::g3d::StreamBuffer::ptr() {
    return map + head;
}

void gear::g3d::StreamBuffer::advanceHead(size_t bytes) {
    head += bytes;
}


