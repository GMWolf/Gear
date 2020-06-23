//
// Created by felix on 11/01/2020.
//

#ifndef GEAR_CORE_H
#define GEAR_CORE_H

#include <typeindex>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>
#include <tuple>
#include <cstddef>
#include <cstdlib>
#include <variant>

#include "Config.h"
#include "Component.h"
#include "Archetype.h"
#include <gear/Util.h>
#include <array>

namespace gear::ecs {

    struct Chunk {
        const Archetype archetype;
        size_t size = 0;
        void* ptr[MaxComponents]{};

        explicit Chunk(const Archetype& archetype): archetype(archetype) {
        }

        void* getData(ComponentId id);
        void* get(ComponentId componentId, uint16_t index);
    };

    struct Entity {
        Chunk* chunk;
        uint32_t version;
        uint16_t index;
    };

    struct EntityRef {
        Entity* entity = nullptr;
        uint32_t version = 0;
        bool alive() const;

        template<class... T>
        std::tuple<T&...> get() {
            return std::forward_as_tuple(*static_cast<T*>(entity->chunk->get(Component<T>::ID(), entity->index)) ...);
        }
    };

    class EntityPool {
    public:
        void getFreeEntities(Entity** vec, size_t count);

        Entity* getFreeEntity();

        void free(Entity* entity);

        size_t getAllocatedEntityCount();

    private:
        static const size_t entitySlabSize = 512;
        using EntitySlab = std::array<Entity, entitySlabSize>;
        std::vector<std::unique_ptr<EntitySlab>> entitySlabVec;
        std::vector<Entity*> freeEntities;
    };

    class Registry {
    private:
        using ChunkVec = std::vector<std::unique_ptr<Chunk>>;
        using Store = std::unordered_map<Archetype, ChunkVec, Archetype::Hash>;
        Store archetypeChunks;

    public:
        class Iterator;
        class QueryBuilder;

        Registry() = default;
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        void emplaceEntity(const Archetype& archetype, Entity* entity);
        void mutateEntity(Entity* entity, const Archetype& archetype);
        void emplaceComponent(Entity* entity, ComponentId componentId, void* componentPtr);
        void destroyEntity(Entity* entity);

        ArrayRange<Chunk*> queryChunks(const Query& query, Chunk** outChunks, size_t outArraySize);
        QueryBuilder query(const Query& q = {});

        Chunk* createChunk(const Archetype& a);
        Chunk* getFreeChunk(const Archetype& a);
    };


    class Registry::Iterator {
        Query query;
        Store::iterator mapit;
        Store::iterator mapend;
        ChunkVec::iterator vecit;

    public:
        Iterator(const Query& q, Store& s);
        explicit Iterator(Store& s);
        Iterator& operator++();
        bool operator==(const Iterator& o) const;
        bool operator!=(const Iterator& o) const;
        Chunk* operator*() const;
    };

    class Registry::QueryBuilder {
        Query query{};
        Registry::Store& store;
    public:
        QueryBuilder(const Query& query, Registry::Store& store);

        [[nodiscard]] inline Iterator begin() const {
            return Iterator{query, store};
        }
        [[nodiscard]] inline Iterator end() const {
            return Iterator{store};
        }

        template<class... T>
        QueryBuilder& all() {
            query.all<T...>();
            return *this;
        }

        template<class... T>
        QueryBuilder& one() {
            query.one<T...>();
            return *this;
        }

        template<class... T>
        QueryBuilder& none() {
            query.none<T...>();
            return *this;
        }
    };




}
#endif //GEAR_CORE_H
