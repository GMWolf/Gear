//
// Created by felix on 28/03/2020.
//

#include <gear/ECS/CommandBuffer.h>
#include <gear/ECS/Component.h>
#include <gear/ECS/Core.h>
#include <memory>

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

    void CommandEncoder::destroyEntity(const EntityRef& entity) {
        write(CommandType::DestroyEntity);
        write(entity);
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

    void executeCreate(CommandDecoder& cmd, Registry& registry) {
        auto archetype = *cmd.read<Archetype>();

        //Create entity and move components
        auto* entity = registry.emplaceEntity(archetype);
        size_t componentCount = archetype.bits.count();
        for(int i = 0; i < componentCount; i++) {
            auto [componentId, componentPtr] = cmd.readComponent();
            registry.emplaceComponent(entity, componentId, componentPtr);
        }
    }


    void executeDestroy(CommandDecoder& cmd, Registry& registry) {
        auto entityRef = cmd.read<EntityRef>();

        if (entityRef->alive()) {
            registry.destroyEntity(entityRef->entity);
        }
    }

    void executeCreateComponent(CommandDecoder& cmd, Registry& registry) {
        auto entityRef = cmd.read<EntityRef>();
        auto[componentId, componentPtr] = cmd.readComponent();
        if (entityRef->alive() && !entityRef->entity->chunk->archetype[componentId]) {
            registry.mutateEntity(entityRef->entity, entityRef->entity->chunk->archetype | componentId);
            registry.emplaceComponent(entityRef->entity, componentId, componentPtr);
        }
    }

    void executeDestroyComponent(CommandDecoder& cmd, Registry& registry) {
        auto entityRef = cmd.read<EntityRef>();
        auto componentId = cmd.read<ComponentId>();
        if (entityRef->alive()) {
            auto a = entityRef->entity->chunk->archetype / *componentId;
            registry.mutateEntity(entityRef->entity, a);
        }
    }

    void executeCommandBuffer(CommandBuffer &buffer, Registry& registry) {
        CommandDecoder cmd(buffer);
        for(int i = 0; i < buffer.commandCount; i++) {
            if (auto* c = cmd.read<CommandType>()) {
                CommandType command = *c;
                switch(command) {
                    case CommandType::CreateEntity:
                        executeCreate(cmd, registry);
                        break;
                    case CommandType::DestroyEntity:
                        executeDestroy(cmd, registry);
                        break;
                    case CommandType::CreateComponent:
                        executeCreateComponent(cmd, registry);
                        break;
                    case CommandType::DestroyComponent:
                        executeDestroyComponent(cmd, registry);
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
        auto id = cmd.read<EntityRef>();
    }

    void resetCreateComponent(CommandDecoder& cmd) {
        cmd.read<EntityRef>();
        cmd.readComponent();
    }

    void resetDestroyComponent(CommandDecoder& cmd) {
        cmd.read<EntityRef>();
        cmd.read<ComponentId>();
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
                    case CommandType::CreateComponent:
                        resetCreateComponent(cmd);
                        break;
                    case CommandType::DestroyComponent:
                        resetDestroyComponent(cmd);
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