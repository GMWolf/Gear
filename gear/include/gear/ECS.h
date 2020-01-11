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
#include <any>

namespace gear::ecs {

    const size_t ChunkSize = 1024;

    class Archetype {
    public:
        Archetype(std::initializer_list<std::type_index> t);

        template<class... A>
        static Archetype create();

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

    class BaseComponentArray {
    public:
        virtual void emplace(size_t index) {
        }

        virtual ~BaseComponentArray() = default;

    };

    template<class T>
    class ComponentArray : public BaseComponentArray {
        std::array<std::aligned_storage_t<sizeof(T), alignof(T)>, ChunkSize> array;

        template<class Tuple, size_t... Is>
        void emplace_impl(size_t index, Tuple &&tuple, std::index_sequence<Is...>) {
            new(array.data() + index) T(std::get<Is>(std::forward<Tuple>(tuple))...);
        }

    public:

        ~ComponentArray() override = default;

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

        void emplace(size_t index) override {
            new(array.data() + index) T();
        }

        T &operator[](size_t index) {
            return *reinterpret_cast<T *>(&array[index]);
        }
    };


    template<class... T>
    class ChunkIterator {
        std::tuple<T *...> ptrs;

    public:

        explicit ChunkIterator(T *... ptrs) : ptrs(ptrs...) {
        }

        std::tuple<T &...> operator*() {
            return std::forward_as_tuple(*std::get<T *>(ptrs)...);
        }

        ChunkIterator operator+(size_t i) {
            return ChunkIterator(std::get<T *>(ptrs) + i ...);
        }

        ChunkIterator &operator++() {
            (++std::get<T *>(ptrs), ...);
            return *this;
        }

        ChunkIterator operator++(int) {
            auto t = *this;
            ++*this;
            return t;
        }

        bool operator==(const ChunkIterator &o) const {
            return ((std::get<T *>(ptrs) == std::get<T *>(o.ptrs)) && ...);
        }

        bool operator!=(const ChunkIterator &o) const {
            return !(*this == o);
        }
    };

    template<class... T>
    class ChunkView {
        std::tuple<ComponentArray<T> &...> arrays;
        size_t size;

    public:

        explicit ChunkView(size_t size, ComponentArray<T> &... arrays) : size(size), arrays(arrays...) {
        }

        ChunkIterator<T...> begin() {
            return ChunkIterator(std::get<ComponentArray<T> &>(arrays).data()...);
        }

        ChunkIterator<T...> end() {
            return begin() + size;
        }

        std::tuple<T &...> operator[](size_t i) {
            return std::forward_as_tuple(std::get<ComponentArray<T> &>(arrays)[i]...);
        }
    };

    class Chunk {
        std::map<std::type_index, std::unique_ptr<BaseComponentArray>> arrays;
        size_t size = 0;
    public:

        template<class T>
        void addArray() {
            arrays.emplace(std::make_pair(
                    std::type_index(typeid(T)),
                    std::make_unique<ComponentArray<T>>()
            ));
        }

        template<class... T>
        ChunkView<T...> view() {
            return ChunkView<T...>(size, *static_cast<ComponentArray<T>*>(arrays[std::type_index(typeid(T))].get())...);
        }


        void emplace_back() {
            for(auto& [type, array] : arrays) {
                array->emplace(size);
            }
            size++;
        }


    };


    namespace bloobiboulga {

        template<class... Components>
        class Chunk {
            std::tuple<ComponentArray<Components>...> arrays;
            size_t m_size = 0;

        public:
            template<class... Args>
            void emplace_back(Args &&... args) {
                static_assert(sizeof...(Components) == sizeof...(Args));
                (std::get<ComponentArray<Components>>(arrays).emplace(m_size, std::forward<Args>(args)), ...);
                m_size++;
            }

            void emplace_back() {
                (std::get<ComponentArray<Components>>(arrays).emplace(m_size), ...);
                m_size++;
            }

            auto operator[](size_t index) {
                return std::forward_as_tuple(std::get<ComponentArray<Components>>(arrays)[index]...);
            }

            ~Chunk() {
                for (size_t i = 0; i < m_size; i++) {
                    (std::get<ComponentArray<Components>>(arrays)[i].~Components(), ...);
                }
            }

            size_t size() {
                return m_size;
            }
        };

        class BaseArchetypeStore {
        public:
            const Archetype archetype;

            explicit BaseArchetypeStore(Archetype &&a) : archetype(std::move(a)) {
            }

            virtual ~BaseArchetypeStore() = default;
        };


        template<class... Components>
        class ArchetypeStore : public BaseArchetypeStore {
            std::vector<std::unique_ptr<Chunk<Components...>>> vec;

        public:
            ArchetypeStore() {};

            ~ArchetypeStore() override = default;

            template<class... Tuples>
            void create(Tuples &&...t) {
                for (int i = 0; i < vec.size(); i++) {
                    if (vec[i]->size() < ChunkSize) {
                        vec[i]->emplace_back(std::forward(t)...);
                    }
                }

                vec.resize(vec.size() + 1);
                vec.back()->emplace_back(std::forward(t)...);
            }

        };

        class World {
            std::map<Archetype, std::unique_ptr<BaseArchetypeStore>> stores;
        public:

            template<class... T, class... Tuples>
            void create(Tuples &&... t) {

                Archetype a = Archetype({std::type_index(typeid(T))...});
                auto f = stores.find(a);


                if (f == stores.end()) {
                    f = stores.emplace(std::make_pair(a, std::make_unique<ArchetypeStore<T...>>())).first;
                }

                static_cast<ArchetypeStore<T...> *>(f->second.get())->create(std::forward<Tuples>(t)...);
            }

        };
    }


}
#endif //GEAR_ECS_H
