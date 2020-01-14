//
// Created by felix on 11/01/2020.
//

#ifndef GEAR_ECS_H
#define GEAR_ECS_H

#include <typeindex>
#include <utility>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <array>
#include <memory>
#include <tuple>
#include <utility>
#include <map>
#include <cstddef>
#include <cstdlib>

namespace gear::ecs {

    using entityId = uint32_t;
    using chunkId = uint32_t;


    const size_t ChunkSize = 1024;

    class Archetype {
    public:
        Archetype(std::initializer_list<std::type_index> t);

        template<class... A>
        static Archetype create();

        bool operator<(const Archetype& o) const {
            return types < o.types;
        }


        template<class... T>
        bool matches() const {
            return ((std::lower_bound(types.begin(), types.end(), std::type_index(typeid(T))) != types.end()) && ...);
        }
    private:
        std::vector<std::type_index> types;
    };

    template<class... A>
    Archetype Archetype::create() {
        return Archetype({std::type_index(typeid(A))...});
    }

    Archetype::Archetype(std::initializer_list<std::type_index> t) : types(t) {
        std::sort(types.begin(), types.end());
    }

    template<class T>
    class ComponentChunk {
        std::array<std::aligned_storage_t<sizeof(T), alignof(T)>, ChunkSize> array;

        template<class Tuple, size_t... Is>
        void emplace_impl(size_t index, Tuple &&tuple, std::index_sequence<Is...>) {
            new(array.data() + index) T(std::get<Is>(std::forward<Tuple>(tuple))...);
        }

    public:

        T *data() {
            return reinterpret_cast<T *>(array.data());
        }

        const T *data() const {
            return reinterpret_cast<T *>(array.data());
        }

        template<class Tuple>
        void emplace(size_t index, Tuple &&tuple) {
            emplace_impl(index, std::forward<Tuple>(tuple),
                         std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
        }

        void emplace(size_t index) {
            new(array.data() + index) T();
        }

        T &operator[](size_t index) {
            return *reinterpret_cast<T *>(&array[index]);
        }
    };

    template<class... T>
    class ComponentIterator {
        std::tuple<T *...> ptrs;

    public:

        explicit ComponentIterator(T *... ptrs) : ptrs(ptrs...) {
        }

        std::tuple<T &...> operator*() {
            return std::forward_as_tuple(*std::get<T *>(ptrs)...);
        }

        ComponentIterator operator+(size_t i) {
            return ComponentIterator(std::get<T *>(ptrs) + i ...);
        }

        ComponentIterator &operator++() {
            (++std::get<T *>(ptrs), ...);
            return *this;
        }

        ComponentIterator operator++(int) {
            auto t = *this;
            ++*this;
            return t;
        }

        bool operator==(const ComponentIterator &o) const {
            return ((std::get<T *>(ptrs) == std::get<T *>(o.ptrs)) && ...);
        }

        bool operator!=(const ComponentIterator &o) const {
            return !(*this == o);
        }
    };

    template<class... T>
    class ChunkView {
        std::tuple<ComponentChunk<T> &...> arrays;
        size_t size;

    public:

        explicit ChunkView(size_t size, ComponentChunk<T> &... arrays) : size(size), arrays(arrays...) {
        }

        ComponentIterator<T...> begin() {
            return ComponentIterator(std::get<ComponentChunk<T> &>(arrays).data()...);
        }

        ComponentIterator<T...> end() {
            return begin() + size;
        }

        std::tuple<T &...> operator[](size_t i) {
            return std::forward_as_tuple(std::get<ComponentChunk<T> &>(arrays)[i]...);
        }
    };

    class ComponentStoreBase {
    public:
        virtual ~ComponentStoreBase() = default;
    };


    template<class T>
    class ComponentStore : public ComponentStoreBase {
    public:
        std::vector<std::unique_ptr<ComponentChunk<T>>> vec;

        ~ComponentStore() override = default;
    };


    class World {
        std::map<std::type_index, std::unique_ptr<ComponentStoreBase>> stores;
        std::vector<size_t> chunkSizes;
        std::map<Archetype, std::vector<chunkId>> archetypeChunks;
        std::vector<entityId> entityIds;
    public:

        [[nodiscard]] size_t getFreeChunk(const Archetype& a) const {
            auto f = archetypeChunks.find(a);
            if (f != archetypeChunks.end()) {
                for (auto c : f->second) {
                    if (chunkSizes[c] < ChunkSize) {
                        return c;
                    }
                }
            }

            return chunkSizes.size();
        }

        template<class T>
        void createComponentChunk(size_t chunk) {
            std::unique_ptr<ComponentStoreBase>& ptr = stores[std::type_index(typeid(T))];

            if (ptr == nullptr) {
                auto cs = std::make_unique<ComponentStore<T>>();
                cs->vec.resize(chunk+1);

                cs->vec[chunk] = std::make_unique<ComponentChunk<T>>();

                ptr = std::move(cs);
            }

        }

        template<class T>
        ComponentChunk<T>& getChunk(size_t chunk) {
            return *static_cast<ComponentStore<T>*>(stores[std::type_index(typeid(T))].get())->vec[chunk].get();
        }

        template<class... T>
        entityId create() {
            Archetype a = Archetype::create<T...>();

            //find a free chunk
            size_t chunk = getFreeChunk(a);

            //create chunk if needed
            if (chunk == chunkSizes.size()) {
                (createComponentChunk<T>(chunk), ...);
                chunkSizes.resize(chunk + 1, 0);
                archetypeChunks[a].push_back(chunk);
            }

            //add entity at end of chunk
            auto size = chunkSizes[chunk];
            (getChunk<T>(chunk).emplace(size), ...);
            chunkSizes[chunk]++;

            entityId internalId = chunk * ChunkSize + size;

            entityIds.push_back(internalId);
            return entityIds.size() - 1;
        }

        template<class... T>
        struct WorldView {
            using chunkIditerator = decltype(archetypeChunks)::mapped_type::iterator;
            chunkIditerator chunkIdBegin;
            chunkIditerator chunkIdEnd;

            std::tuple<ComponentStore<T> *...> stores;
            size_t *chunkSizes;

            struct ChunkIter {
                chunkIditerator c;
                size_t *chunkSizesPtr;
                std::tuple<std::unique_ptr<ComponentChunk<T>>*...> stores;

                ChunkView<T...> operator*() {
                    chunkId chunk = *c;
                    return ChunkView<T...>(chunkSizesPtr[*c], *std::get<std::unique_ptr<ComponentChunk<T>> *>(stores)->get()...);
                }

                bool operator!=(const ChunkIter& o) {
                    return c != o.c;
                }

                ChunkIter& operator++(){
                    c++;
                    return *this;
                }
            };


            ChunkIter begin() {
                return ChunkIter{
                        chunkIdBegin,
                        chunkSizes,
                        std::make_tuple(std::get<ComponentStore<T>*>(stores)->vec.data()...)
                };
            }

            ChunkIter end() {
                return ChunkIter{chunkIdEnd,
                        chunkSizes,
                        std::make_tuple(std::get<ComponentStore<T>*>(stores)->vec.data()...)
                };
            }
        };

        template<class... T>
        WorldView<T...> getChunks() {
            Archetype a = Archetype::create<T...>();

            auto& chunks = archetypeChunks[a];

            return WorldView<T...>{
                chunks.begin(),
                chunks.end(),
                std::make_tuple(static_cast<ComponentStore<T>*>(stores[std::type_index(typeid(T))].get())...),
                chunkSizes.data()
            };
        }


        template<class T>
        T& get(entityId entity) {
            entityId internalId = entityIds[entity];
            entityId inChunk = internalId % ChunkSize;
            chunkId chunk = internalId / ChunkSize;

            ComponentStore<T>* store = static_cast<ComponentStore<T>*>(stores[std::type_index(typeid(T))].get());
            return (*store->vec[chunk])[inChunk];
        }

    };

}
#endif //GEAR_ECS_H
