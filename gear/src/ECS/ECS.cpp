//
// Created by felix on 08/02/2020.
//

#include <gear/ECS/ECS.h>
#include <gear/ECS/Component.h>
#include <iostream>

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

    std::pair<Chunk *, uint16_t> Registry::emplaceEntity(const Archetype &archetype) {
        auto chunk = getFreeChunk(archetype);
        if (!chunk) {
            chunk = createChunk(archetype);
        }

        chunk->size++;

        return std::make_pair(chunk, chunk->size - 1);
    }


    void World::execute(const CreateCommand &createCommand) {
        auto [chunk, eid] = registry.emplaceEntity(createCommand.archetype);
        for(auto& [componentId, componentPointer] : createCommand.components) {
            ComponentInfo::component[componentId].functions.emplace(chunk->get(componentId, eid), componentPointer);
        }

        EntityId entityid = nextComponentId++;
        chunk->entity[eid].id = entityid;
        if (entities.size() <= entityid)
            entities.resize(entityid + 1);

        entities[entityid] = std::make_pair(chunk, eid);
    }

    void World::executeCommandBuffer(const CommandBuffer &commandBuffer) {
        for(auto& c : commandBuffer.commands) {
            execute(std::get<CreateCommand>(c));
        }
    }

    void *Chunk::getData(ComponentId id) {
        return ptr[id];
    }

    void *Chunk::get(ComponentId componentId, uint16_t index) {
        return (char*)getData(componentId) + (index * ComponentInfo::component[componentId].size);
    }

}