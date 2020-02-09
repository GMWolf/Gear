//
// Created by felix on 08/02/2020.
//

#include <gear/ECS/ECS.h>
#include <gear/ECS/Component.h>

namespace gear::ecs {

    ComponentId nextComponentId = 0;


    std::optional<ComponentStore>& Registry::getStore(ComponentId id) {
        return stores[id];
    }

    std::pair<ChunkId, uint16_t> Registry::getFreeIndex(const Archetype &a) const {
        auto s = archetypeChunks.find(a);
        if (s != archetypeChunks.end()) {
            auto& [key, chunks] = *s;
            for(ChunkId chunkid : chunks) {
                if (chunkSize[chunkid] < ChunkSize) {
                    return std::make_pair(chunkid, chunkSize[chunkid]);
                }
            }
        }

        return std::make_pair(chunkSize.size(), 0);
    }

    void Registry::createChunk(ComponentId componentId, ChunkId chunkId) {
        if (!stores[componentId]) {
            createStore(componentId);
        }

        if (!stores[componentId]->hasChunk(chunkId)) {
            stores[componentId]->createChunk(chunkId);
        }
    }

    void ComponentStore::createChunk(ChunkId id) {
        if (chunks.size() <= id) {
            chunks.resize(id + 1, nullptr);
        }

        if (chunks[id] == nullptr) {
            chunks[id] = malloc(componentInfo.size * ChunkSize);
        }
    }

    void ComponentStore::freeChunk(ChunkId id) {
        free(chunks[id]);
        chunks[id] = nullptr;
    }

    bool ComponentStore::hasChunk(ChunkId id) {
        if (id >= chunks.size()) {
            return false;
        }
        return chunks[id] != nullptr;
    }

    void *ComponentStore::getItem(ChunkId chunk, uint16_t index) {
        return (char*)chunks[chunk] + (index * componentInfo.size);
    }

    void ComponentStore::move(ChunkId chunk, uint16_t from, uint16_t to) {
        moveFunc(getItem(chunk, from), getItem(chunk, to));
    }

    void ComponentStore::emplace(ChunkId chunk, uint16_t index, void *from) {
        emplaceFunc(getItem(chunk, index), from);
    }

    void Registry::createStore(ComponentId id) {
        if (!stores[id]) {
            stores[id] = ComponentStore(id);
        }
    }

    std::pair<ChunkId, uint16_t> Registry::createEntity(const Archetype &archetype) {

        auto [chunkId, index] = getFreeIndex(archetype);

        for(ComponentId componentId = 0; componentId < MaxComponents; componentId++) {
            if (archetype[componentId]) {
                if (!getStore(componentId)) {
                    createStore(componentId);
                }

                auto& store = *getStore(componentId);
                if (!store.hasChunk(chunkId)) {
                    store.createChunk(chunkId);
                }
            }
        }

        return std::make_pair(chunkId, index);
    }

    void World::execute(const CreateCommand &createCommand) {
        auto [chunkId, eid] = registry.createEntity(createCommand.archetype);

        for(auto& [componentId, ptr] : createCommand.components) {
            registry.getStore(componentId)->emplace(chunkId, eid, ptr.get());
        }
    }

    void World::executeCommandBuffer(const CommandBuffer &commandBuffer) {
        for(auto& c : commandBuffer.commands) {

            execute(std::get<CreateCommand>(c));

        }
    }
}