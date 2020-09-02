//
// Created by felix on 28/03/2020.
//

#ifndef GEAR_COMMANDBUFFER_H
#define GEAR_COMMANDBUFFER_H


#include "Archetype.h"
#include "Core.h"
#include <cstddef>

namespace gear::ecs {

    enum class CommandType : uint8_t {
        CreateEntity,
        DestroyEntity,
        CreateComponent,
        DestroyComponent,
    };

    class CommandBuffer {

    public:

        EntityPool& entityPool;
        std::byte* buffer {nullptr};
        size_t bufferSize {0};
        size_t commandCount {0};

    private:
        void* head;
        size_t space;

        void *allocate(size_t size, size_t align);

        template<class T>
        T *allocate();

        template<class T>
        bool write(const T& t);

    public:

        CommandBuffer(EntityPool& pool, size_t size);

        template<class... T>
        EntityRef createEntity(T&&... t);
        template<class... T>
        EntityRef createEntityPrefab(EntityRef prefab, T&&... t);
        void destroyEntity(const EntityRef& entity);
        template<class T>
        void createComponent(const EntityRef& entity, T&& t);
        void reset();
        template<class T>
        void destroyComponent(const EntityRef& entity);

        bool writeComponentMove(ComponentId id, void *cptr);
        bool writeComponentCopy(ComponentId id, const void *cptr);
    };

    template<class T>
    T *CommandBuffer::allocate() {
        return reinterpret_cast<T *>(allocate(sizeof(T), alignof(T)));
    }

    template<class T>
    bool CommandBuffer::write(const T& t) {
        if (T* ptr = allocate<T>()) {
            *ptr = t;
            return true;
        } else {
            return false;
        }
    }

    template<class... T>
    EntityRef CommandBuffer::createEntity(T &&... t) {
        static_assert(!std::conjunction_v<std::is_const<T>...>); // we are about to move, so cannot be const
        Entity* entity = entityPool.getFreeEntity();
        write(CommandType::CreateEntity);
        write(entity);
        write(Archetype::create<std::remove_reference_t<T>...>());
        (writeComponentMove(Component<std::remove_reference_t<T>>::ID(), &t), ...);
        commandCount++;
        return EntityRef{
            entity,
            entity->version
        };
    }

    template<class... T>
    EntityRef CommandBuffer::createEntityPrefab(EntityRef prefab, T &&... t) {

        Entity* entity = entityPool.getFreeEntity();
        write(CommandType::CreateEntity);
        write(entity);

        Archetype extraArchetype = Archetype::create<T...>();
        Archetype prefabArchetype = prefab.alive() ? prefab.entity->chunk->archetype / Component<EntityRef>::ID() : Archetype{};
        Archetype archetype = prefabArchetype | extraArchetype;
        write(archetype);

        //Write prefab components not overriten by extras
        for(int i = 0; i < prefabArchetype.bits.size(); i++) {
            if (prefabArchetype[i] && !extraArchetype[i]) {
                writeComponentCopy(i, prefab.entity->chunk->get(i, prefab.entity->index));
            }
        }
        //write extras
        (writeComponentMove(Component<std::remove_reference_t<T>>::ID(), &t), ...);

        commandCount++;
        return EntityRef{
                entity,
                entity->version
        };
    }

    template<class T>
    void CommandBuffer::createComponent(const EntityRef &entity, T&& t) {
        static_assert( !std::is_const_v<T>, "Component cannot be const." );
        write(CommandType::CreateComponent);
        write(entity);
        writeComponentMove(Component<std::remove_reference_t<T>>::ID(), &t);
        commandCount++;
    }

    template<class T>
    void CommandBuffer::destroyComponent(const EntityRef &entity) {
        write(CommandType::DestroyComponent);
        write(entity);
        write(Component<T>::ID());
        commandCount++;
    }

    class Registry;

    void executeCommandBuffer(CommandBuffer& cmd, Registry& world);

}

#endif //GEAR_COMMANDBUFFER_H
