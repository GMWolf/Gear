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
#include <any>

#include "Config.h"
#include "Component.h"
#include "Archetype.h"
#include <cassert>

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


    template<class... T>
    class ChunkIterator {
        std::tuple<T*...> ptr;
    public:
        explicit ChunkIterator(T*... ptrs) : ptr(ptrs...) {}

        std::tuple<T&...> operator*() noexcept {
            return std::forward_as_tuple(*std::get<T*>(ptr)...);
        }

        ChunkIterator operator+(size_t i) const noexcept {
            return ChunkIterator(std::get<T*>(ptr) + i ...);
        }

        ChunkIterator& operator++() noexcept {
            (++std::get<T*>(ptr),...);
            return *this;
        }

        ChunkIterator operator++(int) &noexcept {
            auto t = *this;
            ++*this;
            return t;
        }

        bool operator==(const ChunkIterator& o) {
            return (std::get<0>(ptr) == std::get<0>(o.ptr));
        }

        bool operator!=(const ChunkIterator& o) {
            return !(*this == o);
        }
    };

    template<class... T>
    class ChunkView {
        Chunk& chunk;
    public:

        explicit ChunkView(Chunk& chunk) : chunk(chunk){
        }

        ChunkIterator<T...> begin() {
            return ChunkIterator<T...>( static_cast<T*>(chunk.get(Component<T>::ID(), 0))...);
        }

        ChunkIterator<T...> end() {
            return ChunkIterator<T...>(static_cast<T*>(chunk.get(Component<T>::ID(), chunk.size))...);
        }
    };

    class RegistryIterator {
        Registry::Store::iterator archetypeIt;
        Registry::Store::iterator archetypeEnd;
        Registry::ChunkVec::iterator chunkIt;
        Archetype archetype;
    public:

        RegistryIterator(Registry::Store::iterator archetypeIt, Registry::Store::iterator archetypeEnd, Registry::ChunkVec::iterator chunkIt, const Archetype& archetype);;

        bool operator==(const RegistryIterator& o) const;
        bool operator!=(const RegistryIterator& o) const;

        Chunk& operator*();

        RegistryIterator& operator++();

        RegistryIterator operator++(int)&;
    };


    class RegistryView {
        Registry& registry;
        Archetype archetype;
    public:

        explicit RegistryView(Registry& registry, const Archetype& archetype);

        RegistryIterator begin();
        RegistryIterator end();
    };

    struct CreateCommand {

        CreateCommand() = default;
        CreateCommand(const CreateCommand&) = delete;
        CreateCommand(CreateCommand&&) noexcept;
        CreateCommand& operator=(const CreateCommand&) = delete;

        Archetype archetype{};
        std::vector<std::pair<ComponentId, void*>> components{};
        ~CreateCommand();
    };

    struct DestroyCommand {
        EntityId entityId;
    };

    struct CommandBuffer {
        std::vector<std::variant<CreateCommand, DestroyCommand>> commands;
        template<class... T>
        void createEntity(T&&... t);

        void destroyEntity(const Entity& entity);
    };

    template<class... T>
    void CommandBuffer::createEntity(T&&... t) {
        commands.emplace_back();
        commands.back().emplace<CreateCommand>();
        assert(std::holds_alternative<CreateCommand>(commands.back()));
        auto& createCommand = std::get<CreateCommand>(commands.back());
        assert(createCommand.components.empty());
        createCommand.archetype = Archetype::create<std::remove_reference_t<T>...>();

        (createCommand.components.push_back(std::make_pair(Component<std::remove_reference_t<T>>::ID(), static_cast<void*>(new std::remove_reference_t<T>(t)))), ...);
        assert(createCommand.components.size() == sizeof...(T));
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


        template<class... T, class Fun>
        void foreachChunk(Fun fun) {
            Archetype archetype = Archetype::create<T...>();
            for(auto& [arch, chunks] : registry.archetypeChunks) {
                if (arch.matches(archetype)) {
                    for(auto& chunk : chunks) {
                        fun(ChunkView<T...>(*chunk.get()));
                    }
                }
            }
        }


        RegistryView getChunks(const Archetype& archetype);

        template<class... T>
        std::tuple<T&...> get(Entity entity) {
            auto [chunk, index] = entities[entity.id];
            return std::forward_as_tuple(*static_cast<T*>(chunk->get(Component<T>::ID(), index)) ...);
        }

    };




}
#endif //GEAR_ECS_H