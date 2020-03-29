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

        template<class... T>
        std::tuple<T&...> get() {
            return std::forward_as_tuple(*static_cast<T*>(entity->chunk->get(Component<T>::ID(), entity->index)) ...);
        }
    };



    class Registry {
    public:
        Registry() = default;
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        Entity* emplaceEntity(const Archetype& archetype);
        void emplaceComponent(Entity* entity, ComponentId componentId, void* componentPtr);
        void destroyEntity(Entity* entity);

        ArrayRange<Chunk*> queryChunks(const Query& query, Chunk** outChunks, size_t outArraySize);

    private:
        using ChunkVec = std::vector<std::unique_ptr<Chunk>>;
        using Store = std::unordered_map<Archetype, ChunkVec, Archetype::Hash>;
        Store archetypeChunks;

        static const size_t entitySlabSize = 512;
        using EntitySlab = std::array<Entity, entitySlabSize>;
        std::vector<std::unique_ptr<EntitySlab>> entitySlabVec;
        std::vector<Entity*> freeEntities;

        Chunk* createChunk(const Archetype& a);
        Chunk* getFreeChunk(const Archetype& a);

        Entity* getFreeEntity();
    };


}
#endif //GEAR_CORE_H
