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


    template<class T>
    class ComponentArray {
        std::array<std::aligned_storage_t<sizeof(T), alignof(T)>, ChunkSize> array;

        template<class Tuple, size_t... Is>
        void emplace_impl(size_t index, Tuple&& tuple, std::index_sequence<Is...>) {
            new (array.data() + index) T(std::get<Is>(std::forward<Tuple>(tuple))...);
        }

        public:

        template<class Tuple>
        void emplace(size_t index, Tuple&& tuple) {
            emplace_impl(index, std::forward<Tuple>(tuple),
                    std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
        }

        void emplace(size_t index) {
            new(array.data() + index) T();
        }

        T& operator[](size_t index) {
            return *reinterpret_cast<T*>(&array[index]);
        }
    };

    template<class... Components>
    class Chunk {
            std::tuple<ComponentArray<Components>...> arrays;
            size_t m_size = 0;

        public:
            template<class... Args>
            void emplace_back(Args&&... args) {
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
                for(size_t i = 0; i < m_size; i++) {
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

        explicit BaseArchetypeStore(Archetype&& a) : archetype(std::move(a)) {
        }

        virtual ~BaseArchetypeStore() = default;
    };

    template<class... Components>
    class ArchetypeStore : public BaseArchetypeStore{
        std::vector<std::unique_ptr<Chunk<Components...>>> vec;

    public:
        ArchetypeStore() {};
        ~ArchetypeStore() override = default;

        template<class... Tuples>
        void create(Tuples&& ...t) {
            for(int i = 0; i < vec.size(); i++) {
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
        void create(Tuples&&... t) {

            Archetype a = Archetype({std::type_index(typeid(T))...});
            auto f = stores.find(a);


            if (f == stores.end()) {
                f = stores.emplace(std::make_pair(a, std::make_unique<ArchetypeStore<T...>>())).first;
            }

            static_cast<ArchetypeStore<T...>*>(f->second.get())->create(std::forward<Tuples>(t)...);
        }

    };




}
#endif //GEAR_ECS_H
