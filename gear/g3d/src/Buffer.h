//
// Created by felix on 14/11/2020.
//

#ifndef GEAR_BUFFER_H
#define GEAR_BUFFER_H

#include <cstddef>
#include "GlObject.h"
#include <queue>

namespace gear::g3d {

    class Buffer : public GlObject<Buffer> {
    public:
        Buffer(size_t size, GLbitfield flags);

        void release();

        const size_t size;
    };

    class PersistentlyMappedBuffer : public Buffer {
    public:
        explicit PersistentlyMappedBuffer(size_t size);

    protected:
        static const GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        const void* map;
    };

    class RingBuffer : public PersistentlyMappedBuffer {
    public:
        explicit RingBuffer(size_t size);

        struct Block {
            void* ptr;
            size_t size;
            GLsync* sync;
        };

        Block allocateBlock(size_t numBytes, size_t align);
        void bind(GLenum target, GLuint binding, Block& block);

    private:
        size_t writeHead = 0;

        static const size_t maxRegionCount = 1024;
        struct FencedRegion {
            GLsync sync = nullptr;
            size_t start = 0;
            size_t end = 0;
        };
        std::queue<FencedRegion> fencedRegions;
    };

    template<class T, size_t N>
    class UniformBuffer : public PersistentlyMappedBuffer {
    public:
        UniformBuffer();
        T* operator->() {
            return &((T*)map)[index];
        }
        void bind(GLuint binding);
        GLsync* end();

    private:
        size_t index = 0;
        GLsync fence[N]{};
    };

    template<class T, size_t N>
    UniformBuffer<T, N>::UniformBuffer() : PersistentlyMappedBuffer(sizeof(T) * N) {
    }

    template<class T, size_t N>
    void UniformBuffer<T, N>::bind(GLuint binding) {
        glBindBufferRange(GL_UNIFORM_BUFFER, binding, id, sizeof(T) * index, sizeof(T));
    }

    template<class T, size_t N>
    GLsync *UniformBuffer<T, N>::end() {
        GLsync* sync = &fence[index];
        if (++index == N) {
            index = 0;
        }
        if (fence[index]) {
            GLenum result = GL_TIMEOUT_EXPIRED;
            while (result == GL_TIMEOUT_EXPIRED) {
                result = glClientWaitSync(fence[index], GL_SYNC_FLUSH_COMMANDS_BIT, 10);
            }
            glDeleteSync(fence[index]);
            fence[index] = nullptr;
        }

        return sync;
    }


}

#endif //GEAR_BUFFER_H
