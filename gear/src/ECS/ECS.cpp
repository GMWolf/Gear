//
// Created by felix on 08/02/2020.
//

#include <gear/ECS/ECS.h>
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
        for(int i = 0; i < createCommand.componentCount; i++) {
            auto& [componentId, componentPointer] = createCommand.components[i];
            ComponentInfo::component[componentId].functions.emplace(chunk->get(componentId, eid), componentPointer);
        }
        Entity entity;
        entity.id = getFreeEntityId();
        entity.archetype = createCommand.archetype;
        if (entities.size() <= entity.id)
            entities.resize(entity.id + 1);

        entities[entity.id] = std::make_pair(chunk, eid);

        Component<Entity>::Functions::emplace(chunk->get(Component<Entity>::ID(), eid), &entity);
    }

    void World::execute(const DestroyCommand& command) {
        auto [chunk, index] = entities[command.entityId];


        if (chunk && (chunk->size > 0)) {
            uint16_t idFrom = chunk->size - 1;

            EntityId entityIdFrom = static_cast<Entity *>(chunk->get(Component<Entity>::ID(), idFrom))->id;

            for (int i = 0; i < MaxComponents; i++) {
                if (chunk->archetype[i]) {
                    void *from = chunk->get(i, idFrom);
                    void *to = chunk->get(i, index);
                    if (from != to) {
                        ComponentInfo::component[i].functions.move(from, to);
                    }
                    ComponentInfo::component[i].functions.destroy(from);
                }
            }
            chunk->size--;

            static_cast<Entity*>(chunk->get(Component<Entity>::ID(), index))->id = entityIdFrom;

            entities[entityIdFrom].second = index;
            entities[command.entityId].first = nullptr;
        }

    }

    void World::executeCommandBuffer(const CommandBuffer &commandBuffer) {
        for(auto& c : commandBuffer.commands) {
            //std::visit([this](auto c){execute(c);}, c);
            if (std::holds_alternative<DestroyCommand>(c)) {
                execute(std::get<DestroyCommand>(c));
            } else if (std::holds_alternative<CreateCommand>(c)) {
                execute(std::get<CreateCommand>(c));
            }
        }

    }

    EntityId World::getFreeEntityId() {
        for(int i = 0; i < entities.size(); i++) {
            if (entities[i].first == nullptr) {
                return i;
            }
        }
        return nextEntityId++;
    }
    size_t World::queryChunks(const Query &query, Chunk **outChunks, size_t outArraySize) {
        if (outArraySize == 0) {
            return 0;
        }
        size_t count = 0;
        for(auto& [arch, chunks] : registry.archetypeChunks) {
            if (testQuery(query, arch)) {
                for(auto& chunkPtr : chunks) {
                    outChunks[count++] = chunkPtr.get();
                    if (count == outArraySize) {
                        return count;
                    }
                }
            }
        }
        return count;
    }

    void *Chunk::getData(ComponentId id) {
        return ptr[id];
    }

    void *Chunk::get(ComponentId componentId, uint16_t index) {
        return (char*)getData(componentId) + (index * ComponentInfo::component[componentId].size);
    }

    CreateCommand::~CreateCommand() {
        for(int i = 0; i < componentCount; i++) {
            auto [id, ptr] = components[i];
            ComponentInfo::component[id].functions.destroy(ptr);
        }
    }

    CreateCommand::CreateCommand(CreateCommand && o) noexcept :
    archetype(o.archetype),
    components(o.components),
    componentCount(o.componentCount){
        o.componentCount = 0;
        o.components = nullptr;
    }
}