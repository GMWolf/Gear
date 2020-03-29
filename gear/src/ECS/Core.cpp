//
// Created by felix on 08/02/2020.
//

#include <gear/ECS/Core.h>
#include <gear/ECS/Component.h>
#include <iostream>
#include <cassert>

namespace gear::ecs {

    ComponentId nextComponentId = 0;

    Chunk* Registry::getFreeChunk(const Archetype &a)  {
        auto s = archetypeChunks.find(a);
        if (s != archetypeChunks.end()) {
            auto& [key, chunks] = *s;
            for(auto& chunk : chunks) {
                if (chunk->size < ChunkSize) {
                    return chunk.get();
                }
            }
        }

        return nullptr;
    }

    Chunk* Registry::createChunk(const Archetype& a) {
        auto&[key, chunks] = *archetypeChunks.try_emplace(a).first;

        Chunk* chunk = chunks.emplace_back(std::make_unique<Chunk>(a)).get();

        for (ComponentId i = 0; i < MaxComponents; i++) {
            if (a[i]) {
                chunk->ptr[i] = malloc(ComponentInfo::component[i].size * ChunkSize);
            }
        }

        return chunk;
    }

    Entity* Registry::emplaceEntity(const Archetype &archetype) {
        Archetype realArchetype = archetype | Component<EntityRef>::ID();
        auto chunk = getFreeChunk(realArchetype);
        if (!chunk) {
            chunk = createChunk(realArchetype);
        }

        uint16_t eid = chunk->size;

        auto* entity = getFreeEntity();
        entity->chunk = chunk;
        entity->index = eid;

        static_cast<EntityRef*>(chunk->get(Component<EntityRef>::ID(), eid))->entity = entity;
        static_cast<EntityRef*>(chunk->get(Component<EntityRef>::ID(), eid))->version = entity->version;

        chunk->size++;
        return entity;
    }

    Entity *Registry::getFreeEntity() {
        if (freeEntities.empty()) {
            auto& slab = entitySlabVec.emplace_back(std::make_unique<EntitySlab>());
            for(auto& e : *slab) {
                e.version = 1;
                e.chunk = nullptr;
                e.index = 0;
                freeEntities.push_back(&e);
            }
        }

        auto ret = freeEntities.back();
        freeEntities.pop_back();
        return ret;
    }

    void Registry::emplaceComponent(Entity *entity, ComponentId componentId, void * componentPtr) {
        ComponentInfo::component[componentId].functions.emplace(entity->chunk->get(componentId, entity->index), componentPtr);
    }

    void Registry::destroyEntity(Entity *entity) {
        Chunk* chunk = entity->chunk;
        uint16_t index = entity->index;

        uint16_t idFrom = chunk->size - 1;
        Entity *entityFrom = static_cast<EntityRef *>(chunk->get(Component<EntityRef>::ID(), idFrom))->entity;
        Entity *entityTo = static_cast<EntityRef *>(chunk->get(Component<EntityRef>::ID(), index))->entity;

        for (int i = 0; i < MaxComponents; i++) {
            if (chunk->archetype[i]) {
                void *from = chunk->get(i, idFrom);
                if (idFrom != index) {
                    void *to = chunk->get(i, index);
                    ComponentInfo::component[i].functions.move(from, to);
                }
                ComponentInfo::component[i].functions.destroy(from);
            }
        }
        chunk->size--;

        //make destroyed entity available
        freeEntities.push_back(entityTo);
        entityTo->chunk = nullptr;
        entityTo->version++;

        if (idFrom != index) {
            //patch moved entity pointer
            entityFrom->chunk = chunk;
            entityFrom->index = index;
        }
    }

    ArrayRange<Chunk *> Registry::queryChunks(const Query &query, Chunk **outChunks, size_t outArraySize) {
        if (outArraySize == 0) {
            return {};
        }
        size_t count = 0;
        for(auto& [arch, chunks] : archetypeChunks) {
            if (testQuery(query, arch)) {
                for(auto& chunkPtr : chunks) {
                    if (count == outArraySize) {
                        std::cerr << "Ran out of chunk buffer space" << std::endl;
                        return {outChunks, outChunks+count};
                    }
                    outChunks[count++] = chunkPtr.get();
                }
            }
        }
        return {outChunks, outChunks+count};
    }

    void *Chunk::getData(ComponentId id) {
        return ptr[id];
    }

    void *Chunk::get(ComponentId componentId, uint16_t index) {
        return (char*)getData(componentId) + (index * ComponentInfo::component[componentId].size);
    }

    bool EntityRef::alive() {
        return version == entity->version;
    }
}