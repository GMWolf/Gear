//
// Created by felix on 11/01/2020.
//

#ifndef GEAR_CORE_H
#define GEAR_CORE_H

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
#include <any>

#include "Config.h"
#include "Component.h"
#include "Archetype.h"
#include <cassert>
#include <gear/Allocators.h>
#include <gear/Util.h>

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


    class Registry {
    public:
        using ChunkVec = std::vector<std::unique_ptr<Chunk>>;
        using Store = std::unordered_map<Archetype, ChunkVec, Archetype::Hash>;
        Store archetypeChunks;

        Registry() = default;
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        Chunk* createChunk(const Archetype& a);
        Chunk* getFreeChunk(const Archetype& a);

        std::pair<Chunk*, uint16_t> emplaceEntity(const Archetype& archetype);
    };


    struct CreateCommand {

        CreateCommand() = default;
        CreateCommand(const CreateCommand&) = delete;
        CreateCommand(CreateCommand&&) noexcept;
        CreateCommand& operator=(const CreateCommand&) = delete;

        Archetype archetype{};
        std::pair<ComponentId, void*>* components{};
        size_t componentCount = 0;

        ~CreateCommand();
    };

    struct DestroyCommand {
        EntityId entityId;
    };

    struct CommandBuffer {

        CommandBuffer() : heap(Block{(std::byte*)malloc(1024*1024), 1024*1024}){
        }

        StackAllocator heap;
        std::vector<std::variant<CreateCommand, DestroyCommand>> commands;
        template<class... T>
        void createEntity(T&&... t);

        void destroyEntity(const Entity& entity);
    };

    template<class... T>
    void CommandBuffer::createEntity(T&&... t) {
        commands.emplace_back();
        commands.back().emplace<CreateCommand>();
        auto& createCommand = std::get<CreateCommand>(commands.back());
        createCommand.archetype = Archetype::create<std::remove_reference_t<T>...>();
        createCommand.components = reinterpret_cast<std::pair<ComponentId, void *> *>(heap.allocate(sizeof(std::pair<ComponentId, void *>) * sizeof...(T)).ptr);
        ((createCommand.components[createCommand.componentCount++] = (std::make_pair(Component<std::remove_reference_t<T>>::ID(),
                static_cast<void*>(new (heap.allocate(Component<std::remove_reference_t<T>>::info().size).ptr) std::remove_reference_t<T>(t))))), ...);
    }

    class World {
        Registry registry;
        EntityId nextEntityId = 0;
        std::vector<std::pair<Chunk*, uint16_t>> entities;
        EntityId getFreeEntityId();

    public:
        void execute(const CreateCommand& createCommand);
        void execute(const DestroyCommand& destroyCommand);
        void executeCommandBuffer(const CommandBuffer& commandBuffer);


        template<class... T>
        std::tuple<T&...> get(Entity entity) {
            auto [chunk, index] = entities[entity.id];
            return std::forward_as_tuple(*static_cast<T*>(chunk->get(Component<T>::ID(), index)) ...);
        }


        ArrayRange<Chunk*> queryChunks(const Query& query, Chunk** outChunks, size_t outArraySize);

    };




}
#endif //GEAR_CORE_H
