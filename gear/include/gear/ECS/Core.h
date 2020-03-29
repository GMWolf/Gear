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
#include <gear/Allocators.h>
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
        Entity* entity;
        uint32_t version;
        bool alive();
    };



    class Registry {
    public:
        using ChunkVec = std::vector<std::unique_ptr<Chunk>>;
        using Store = std::unordered_map<Archetype, ChunkVec, Archetype::Hash>;
        Store archetypeChunks;

        static const size_t entitySlabSize = 512;
        using EntitySlab = std::array<Entity, entitySlabSize>;
        std::vector<std::unique_ptr<EntitySlab>> entitySlabVec;
        std::vector<Entity*> freeEntities;

        Registry() = default;
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        Chunk* createChunk(const Archetype& a);
        Chunk* getFreeChunk(const Archetype& a);

        std::pair<Chunk*, uint16_t> emplaceEntity(const Archetype& archetype);
        Entity* getFreeEntity();
    };

    class World {

    public:
        Registry registry;

        template<class... T>
        std::tuple<T&...> get(EntityRef entity) {
            return std::forward_as_tuple(*static_cast<T*>(entity.entity->chunk->get(Component<T>::ID(), entity.entity->index)) ...);
        }

        ArrayRange<Chunk*> queryChunks(const Query& query, Chunk** outChunks, size_t outArraySize);
    };




}
#endif //GEAR_CORE_H
