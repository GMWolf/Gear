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
            if (a[i] || i == Component<Entity>::ID()) {
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

    void CommandBuffer::destroyEntity(const Entity &entity) {
        commands.emplace_back(DestroyCommand{entity.id});
    }


    void World::execute(const CreateCommand &createCommand) {
        auto [chunk, eid] = registry.emplaceEntity(createCommand.archetype);
        for(auto& [componentId, componentPointer] : createCommand.components) {
            ComponentInfo::component[componentId].functions.emplace(chunk->get(componentId, eid), componentPointer);
        }
        Entity entity;
        entity.id = nextComponentId++;
        if (entities.size() <= entity.id)
            entities.resize(entity.id + 1);
        entities[entity.id] = std::make_pair(chunk, eid);

        Component<Entity>::Functions::emplace(chunk->get(Component<Entity>::ID(), eid), &entity);
    }

    void World::executeCommandBuffer(const CommandBuffer &commandBuffer) {
        for(auto& c : commandBuffer.commands) {
            std::visit([this](auto c){execute(c);}, c);
        }

    }

    void World::execute(const DestroyCommand& command) {
        auto [chunk, index] = entities[command.entityId];

        uint16_t idFrom = chunk->size - 1;

        for(int i = 0; i < MaxComponents; i++) {
            if (chunk->archetype[i]) {
                void* from = chunk->get(i, idFrom);
                void* to = chunk->get(i, index);
                if (from != to) {
                    ComponentInfo::component[i].functions.move(from, to);
                }
                ComponentInfo::component[i].functions.destroy(from);
            }
        }
        chunk->size--;

        Entity entityFrom = *static_cast<Entity*>(chunk->get(Component<Entity>::ID(), idFrom));
        entities[entityFrom.id].second = index;
    }

    void *Chunk::getData(ComponentId id) {
        return ptr[id];
    }

    void *Chunk::get(ComponentId componentId, uint16_t index) {
        return (char*)getData(componentId) + (index * ComponentInfo::component[componentId].size);
    }

}