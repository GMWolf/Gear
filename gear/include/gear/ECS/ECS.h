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
#include <cstddef>
#include <cstdlib>
#include <variant>

#include "Config.h"
#include "Component.h"
#include "Archetype.h"

namespace gear::ecs {

    class ComponentStore {
        std::vector<void*> chunks;
        ComponentInfo componentInfo;

        void (*moveFunc)(void* from, void* to){nullptr};
        void (*emplaceFunc)(void* pos, void* from){nullptr};
        void (*destroy)(void* ptr){nullptr};
    public:

        explicit ComponentStore(ComponentId id) : componentInfo(ComponentInfo::component[id]) {
        }

        void createChunk(ChunkId id);

        void freeChunk(ChunkId id);

        bool hasChunk(ChunkId id);

        void* getItem(ChunkId chunk, uint16_t index);

        void move(ChunkId chunk, uint16_t from, uint16_t to);

        void emplace(ChunkId chunk, uint16_t index, void* from);

    };

    class Registry {
        std::array<std::optional<ComponentStore>, MaxComponents> stores;

        std::unordered_map<Archetype, std::vector<ChunkId>, Archetype::Hash> archetypeChunks;
        std::vector<uint16_t> chunkSize;

    public:

        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        std::optional<ComponentStore>& getStore(ComponentId id);

        std::pair<ChunkId, uint16_t> getFreeIndex(const Archetype& a) const;

        void createStore(ComponentId id);

        void createChunk(ComponentId componentId, ChunkId chunkId);

        std::pair<ChunkId, uint16_t> createEntity(const Archetype& archetype);
    };


    struct CreateCommand {
        Archetype archetype;
        std::vector<std::pair<ComponentId, std::unique_ptr<void>>> components;
    };

    struct CommandBuffer {
        std::vector<std::variant<CreateCommand>> commands;

        template<class... T>
        void createEntity(T&&... t);
    };

    template<class... T>
    void CommandBuffer::createEntity(T&&... t) {
        auto& command = commands.emplace_back(CreateCommand{});
        auto& createCommand = std::get<CreateCommand>(command);
        createCommand.archetype = Archetype::create<T...>();

        (createCommand.components.emplace_back(std::make_pair(Component<std::remove_reference_t<T>>::ID(), std::make_unique<std::remove_reference_t<T>>(t))), ...);
    }

    class World {
        Registry registry;

    public:

        void execute(const CreateCommand& createCommand);

        void executeCommandBuffer(const CommandBuffer& commandBuffer);

    };

}
#endif //GEAR_ECS_H
