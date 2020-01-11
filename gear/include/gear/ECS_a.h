//
// Created by felix on 10/01/2020.
//

#ifndef GEAR_ECS_A_H
#define GEAR_ECS_A_H

#include <vector>
#include <typeindex>
#include <initializer_list>
#include <algorithm>
#include <tuple>
#include <array>
#include <memory>
#include <map>

namespace gear::ecs_a {

        const size_t ChunkSize = 1024;

        extern size_t componentCounter;

        template<class T>
        struct ComponentTrait {
            static size_t id;
        };
        template<class T>
        size_t ComponentTrait<T>::id = componentCounter++;

        class Archetype {
        public:
            Archetype(std::initializer_list<std::type_index> t) : types(t) {
                std::sort(types.begin(), types.end());
            }

            template<class... A>
            static Archetype create();

        private:
            //list of components
            std::vector<std::type_index> types;
        };

        template<class... A>
        Archetype Archetype::create() {
            return Archetype({std::type_index(typeid(A))...});
        }

        template<class... Component>
        class EntityChunkIterator {
                std::tuple<std::array<Component, ChunkSize>*...> pointers;
                size_t* size;

                bool operator==(const EntityChunkIterator& o) const {
                    return std::get<0>(pointers) == std::get<0>(o.pointers0);
                }

                bool operator!= (const EntityChunkIterator& o) const {
                    return !(*this == o);
                }

                std::tuple<size_t, Component*...> operator*(){
                    return std::make_tuple(*size, std::get<std::array<Component, ChunkSize>*>(pointers)->data()...);
                }
        };

        template<class T> using Chunk = std::array<T, ChunkSize>;

        template<class... T> using ChunkPointers = std::tuple<Chunk<T>*...>;

        template<class... Component>
        class ChunkIterator {
            ChunkPointers<Component...> ptrs;
            size_t* sizes;
        public:

            ChunkIterator(ChunkPointers<Component...> ptrs) : ptrs(ptrs){
            }

            std::tuple<Chunk<Component>&&...> operator*() {
                return std::forward_as_tuple(std::get<Component>(ptrs)...);
            }

            ChunkIterator& operator++() {
                (++std::get<Component>(ptrs), ...);
                return *this;
            }

            ChunkIterator operator++(int) {
                const auto temp = *this;
                operator++();
                return temp;
            }
        };

        template<class... Component>
        class StoreView {
            ChunkPointers<Component...> start;
            ChunkPointers<Component...> finish;
        public:
            StoreView(ChunkPointers<Component...> start, ChunkPointers<Component...> finish) : start(start), finish(finish) {
            }

            std::tuple<Chunk<Component>*...> begin() {
                return start;
            }

            std::tuple<Chunk<Component>*...> end() {
                return finish;
            }
        };

        class Store {
            Archetype archetype;
            std::map<std::type_index,std::vector<std::unique_ptr<void>>> stores;

            template<class... Component>
            StoreView<Component...> getView() {
                return {
                    std::make_tuple(
                            std::static_pointer_cast<Chunk<Component>>(
                                    stores[std::type_index(typeid(Component))]
                                    )->begin()...
                                    ),
                    std::make_tuple(
                    std::static_pointer_cast<Chunk<Component>>(
                            stores[std::type_index(typeid(Component))]
                    )->end()...
                        )
                };
            }
        };

        class World {

            std::vector<Store> stores;
        public:


        };

        class EntityStoreBase {
            const Archetype archetype;

            explicit EntityStoreBase(Archetype&& archetype) : archetype(archetype) {}


        };

        template<class... Component>
        class EntityStore : public EntityStoreBase {

            std::tuple<std::vector<std::array<Component, ChunkSize> *>...> chunks;
            std::vector<size_t> chunkSizes;

        public:

            EntityStore() : EntityStoreBase({std::type_index(typeid(Component))...}){
            }


            template<class... C>
            void begin() {
                return std::make_tuple(std::get<C>(chunks).begin()...);
            }

            template<class... C>
            void end() {
                return std::make_tuple(std::get<C>(chunks).end()...);
            }
        };

    }



#endif //GEAR_ECS_A_H
