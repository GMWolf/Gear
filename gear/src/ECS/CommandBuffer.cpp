//
// Created by felix on 28/03/2020.
//

#include <gear/ECS/CommandBuffer.h>
#include <gear/ECS/Component.h>
#include <optional>
#include <gear/ECS/Core.h>

namespace gear::ecs {

    void* CommandEncoder::allocate(size_t size, size_t align) {
        //align the pointer
        if (std::align(align, size, head, space)) {
            void *ret = head;
            head = (char *) head + size;
            space -= size;
            return ret;
        }
        return nullptr;
    }

    bool CommandEncoder::writeComponent(ComponentId id, void *cptr) {
        if (!write(id))
            return false;

        if (void* p = allocate(ComponentInfo::component[id].size, ComponentInfo::component[id].align)) {
            ComponentInfo::component[id].functions.emplace(p, cptr);
            return true;
        }

        return false;
    }

    void CommandEncoder::destroyEntity(Entity entity) {
        write(CommandType::DestroyEntity);
        write(entity.id);
        cmd.commandCount++;
    }

    void CommandEncoder::reset() {
        head = cmd.buffer;
        space = cmd.bufferSize;
    }


    class CommandDecoder {
        void* head;
        size_t space;

        void* read(size_t size, size_t align);

    public:
        explicit CommandDecoder(CommandBuffer& cmd) : head(cmd.buffer), space(cmd.bufferSize) {};

        template<class T>
        T* read();

        std::pair<ComponentId, void*> readComponent();
    };

    void *CommandDecoder::read(size_t size, size_t align) {
        if(std::align(align, size, head, space)) {
            void* ret = head;
            head = (char*)head + size;
            space -= size;
            return ret;
        }
        return nullptr;
    }

    template<class T>
    T *CommandDecoder::read() {
        return reinterpret_cast<T*>(read(sizeof(T), alignof(T)));
    }

    std::pair<ComponentId, void *> CommandDecoder::readComponent() {
        auto id = *read<ComponentId>();
        void* ptr = read(ComponentInfo::component[id].size, ComponentInfo::component[id].align);
        return std::make_pair(id, ptr);
    }

    void executeCreate(CommandDecoder& cmd, World& world) {
        auto archetype = *cmd.read<Archetype>();

        //Create entity and move components
        auto [chunk, eid] = world.registry.emplaceEntity(archetype);
        size_t componentCount = archetype.bits.count();
        for(int i = 0; i < componentCount; i++) {
            auto [componentId, componentPtr] = cmd.readComponent();
            ComponentInfo::component[componentId].functions.emplace(chunk->get(componentId, eid), componentPtr);
        }

        //Update Entity table
        Entity entity;
        entity.id = world.getFreeEntityId();
        entity.archetype = archetype;
        if (world.entities.size() <= entity.id)
            world.entities.resize(entity.id + 1);

        world.entities[entity.id] = std::make_pair(chunk, eid);
        Component<Entity>::Functions::emplace(chunk->get(Component<Entity>::ID(), eid), &entity);
    }


    void executeDestroy(CommandDecoder& cmd, World& world) {
        auto entityId = *cmd.read<EntityId>();

        auto [chunk, index] = world.entities[entityId];

        if (chunk && (chunk->size > 0)) {
            uint16_t idFrom = chunk->size - 1;
            EntityId entityIdFrom = static_cast<Entity*>(chunk->get(Component<Entity>::ID(), idFrom))->id;

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

            static_cast<Entity*>(chunk->get(Component<Entity>::ID(), index))->id = entityIdFrom;
            world.entities[entityIdFrom].second = index;
            world.entities[entityId].first = nullptr;
        }
    }

    void executeCommandBuffer(CommandBuffer &buffer, World& world) {
        CommandDecoder cmd(buffer);
        for(int i = 0; i < buffer.commandCount; i++) {
            if (auto* c = cmd.read<CommandType>()) {
                CommandType command = *c;
                switch(command) {
                    case CommandType::CreateEntity:
                        executeCreate(cmd, world);
                        break;
                    case CommandType::DestroyEntity:
                        executeDestroy(cmd, world);
                        break;
                    default:
                        break;
                }
            }

        }
    }

    void resetCreate(CommandDecoder& cmd) {
        auto archetype = *cmd.read<Archetype>();
        size_t componentCount = archetype.bits.count();
        for(int i = 0; i < componentCount; i++) {
            auto [componentId, componentPtr] = cmd.readComponent();
            ComponentInfo::component[componentId].functions.destroy(componentPtr);
        }
        archetype.~Archetype();
    }

    void resetDestroy(CommandDecoder& cmd) {
        auto id = cmd.read<EntityId>();
    }


    void resetCommandBuffer(CommandBuffer& buffer) {
        CommandDecoder cmd(buffer);
        for (int i = 0; i < buffer.commandCount; i++) {

            if (auto *c = cmd.read<CommandType>()) {
                CommandType command = *c;
                switch (command) {
                    case CommandType::CreateEntity:
                        resetCreate(cmd);
                        break;
                    case CommandType::DestroyEntity:
                        resetDestroy(cmd);
                        break;
                    default:
                        break;
                }
            }
        }

        buffer.commandCount = 0;
    }

    CommandBuffer::CommandBuffer(size_t size) : buffer(static_cast<std::byte *>(malloc(size))), bufferSize(size), commandCount(0) {}
}