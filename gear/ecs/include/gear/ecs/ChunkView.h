//
// Created by felix on 28/03/2020.
//

#ifndef GEAR_CHUNKVIEW_H
#define GEAR_CHUNKVIEW_H


#include "Core.h"

namespace gear::ecs {

    template<class... T>
    class ChunkIterator {
        std::tuple<T*...> ptr;
        int idx;
    public:
        explicit ChunkIterator(T*... ptrs, int idx) : ptr(ptrs...), idx(idx) {}

        std::tuple<T&...> operator*() noexcept {
            return std::forward_as_tuple(std::get<T*>(ptr)[idx]...);
        }

        ChunkIterator operator+(size_t i) const noexcept {
            return ChunkIterator(std::get<T*>(ptr) ..., idx + i);
        }

        ChunkIterator& operator++() noexcept {
            idx++;
            return *this;
        }

        ChunkIterator operator++(int) &noexcept {
            auto t = *this;
            ++*this;
            return t;
        }

        bool operator==(const ChunkIterator& o) {
            return (std::get<0>(ptr)+idx == std::get<0>(o.ptr)+o.idx);
        }

        bool operator!=(const ChunkIterator& o) {
            return !(*this == o);
        }
    };

    template<class... T>
    class ChunkView {
        Chunk& chunk;
    public:

        explicit ChunkView(Chunk& chunk) : chunk(chunk){
        }

        ChunkIterator<T...> begin() {
            return ChunkIterator<T...>( static_cast<T*>(chunk.getData(Component<T>::ID()))..., 0);
        }

        ChunkIterator<T...> end() {
            return ChunkIterator<T...>(static_cast<T*>(chunk.getData(Component<T>::ID()))..., chunk.size);
        }

        template<class... E>
        bool operator==(const ChunkView<E...>& o) const {
            return &chunk == &o.chunk;
        }
    };

}

#endif //GEAR_CHUNKVIEW_H
