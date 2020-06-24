//
// Created by felix on 28/03/2020.
//

#include <gear/ECS/CommandBuffer.h>
#include <gear/ECS/Component.h>
#include <gear/ECS/Core.h>
#include <memory>

namespace gear::ecs {

    void* CommandBuffer::allocate(size_t size, size_t align) {
        //align the pointer
        if (std::align(align, size, head, space)) {
            void *ret = head;
            head = (char *) head + size;
            space -= size;
            return ret;
        }
        return nullptr;
    }

    bool CommandBuffer::writeComponentMove(ComponentId id, void *cptr) {
        if (!write(id))
            return false;

        if (void* p = allocate(ComponentInfo::component[id].size, ComponentInfo::component[id].align)) {
            ComponentInfo::component[id].functions.emplaceMove(p, cptr);
            return true;
        }

        return false;
    }

    bool CommandBuffer::writeComponentCopy(ComponentId id, const void *cptr) {
        if (!write(id))
            return false;

        if (void* p = allocate(ComponentInfo::component[id].size, ComponentInfo::component[id].align)) {
            ComponentInfo::component[id].functions.emplaceCopy(p, cptr);
            return true;
        }

        return false;
    }

    void CommandBuffer::destroyEntity(const EntityRef& entity) {
        write(CommandType::DestroyEntity);
        write(entity);
        commandCount++;
    }

    void CommandBuffer::reset() {
        head = buffer;
        space = bufferSize;
        commandCount = 0;
    }


    class CommandDecoder {
        CommandBuffer& cmd;
        void* head;
        size_t space;

        void* read(size_t size, size_t align);

    public:
        explicit CommandDecoder(CommandBuffer& cmd) : cmd(cmd), head(cmd.buffer), space(cmd.bufferSize) {};

        template<class T>
        T* read();

        std::pair<ComponentId, void*> readComponent();

        CommandBuffer& buffer();
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

    CommandBuffer &CommandDecoder::buffer() {
        return cmd;
    }

    void executeCreate(CommandDecoder& cmd, Registry& registry) {
        Entity* entity = *cmd.read<Entity*>();
        auto& archetype = *cmd.read<Archetype>();

        //Create entity and move components
        registry.emplaceEntity(archetype, entity);
        size_t componentCount = archetype.bits.count();
        for(int i = 0; i < componentCount; i++) {
            auto [componentId, componentPtr] = cmd.readComponent();
            registry.emplaceComponent(entity, componentId, componentPtr);
            ComponentInfo::component[componentId].functions.destroy(componentPtr);
        }

        archetype.~Archetype();
    }


    void executeDestroy(CommandDecoder& cmd, Registry& registry) {
        auto entityRef = cmd.read<EntityRef>();
        if (entityRef->alive()) {
            Entity* e = entityRef->entity;
            registry.destroyEntity(e);
            cmd.buffer().entityPool.free(e);
        }
    }

    void executeCreateComponent(CommandDecoder& cmd, Registry& registry) {
        auto entityRef = cmd.read<EntityRef>();
        auto[componentId, componentPtr] = cmd.readComponent();
        if (entityRef->alive() && !entityRef->entity->chunk->archetype[componentId]) {
            registry.mutateEntity(entityRef->entity, entityRef->entity->chunk->archetype | componentId);
            registry.emplaceComponent(entityRef->entity, componentId, componentPtr);
            ComponentInfo::component[componentId].functions.destroy(componentPtr);
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

        buffer.reset();
    }
    CommandBuffer::CommandBuffer(EntityPool& pool, size_t size) : entityPool(pool), buffer(static_cast<std::byte *>(malloc(size))), bufferSize(size), commandCount(0), head(buffer), space(size) {}
}