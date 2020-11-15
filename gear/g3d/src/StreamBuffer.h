//
// Created by felix on 14/11/2020.
//

#ifndef GEAR_STREAMBUFFER_H
#define GEAR_STREAMBUFFER_H

#include <cstddef>
#include "GlObject.h"

namespace gear::g3d {

    class StreamBuffer : public GlObject<StreamBuffer> {

    public:
        explicit StreamBuffer(size_t size);
        ~StreamBuffer() = default;

        StreamBuffer(StreamBuffer&& other) = delete;
        StreamBuffer& operator=(StreamBuffer&& other) = delete;

        void release();

        [[nodiscard]] size_t available() const;
        bool clientWait();
        void clientWaitUntilAvailable(size_t size);
        void clientWaitAndResetHead();

        [[nodiscard]] inline size_t space() const {
            return size - head;
        }

        void* ptr();

        struct BufferRegion {
            GLsync sync;
            size_t start;
            size_t size;
        };

        BufferRegion* finishWrite();
        void advanceHead(size_t bytes);

    protected:
        uint8_t* map;
        const size_t size;
        size_t head = 0;
        size_t regionStart = 0;
        static const size_t maxFences = 512;

        BufferRegion fences[maxFences];
        size_t writeFence = 0;
        size_t readFence = 0;
    };

}

#endif //GEAR_STREAMBUFFER_H
