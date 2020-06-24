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
    struct ComponentProvider {
        static void writeComponents(T& t, CommandBuffer& cmd);
        static Archetype archetype(const T& t);
    };

    template<class T>
    Archetype ComponentProvider<T>::archetype(const T& t) {
        return Archetype::create<T>();
    }

    template<class T>
    void ComponentProvider<T>::writeComponents(T& t, CommandBuffer& cmd){
        cmd.writeComponentMove(Component<T>::ID(), &t);
    }

    template<class T>
    struct CopyProvider {
            explicit CopyProvider(const T& t);
            const T& t;
    };

    template<class T>
    CopyProvider<T>::CopyProvider(const T &t) : t(t){
    }

    template<class T>
    struct ComponentProvider<CopyProvider<T>> {
        static void writeComponents(CopyProvider<T>& cpy, CommandBuffer& cmd);
        static Archetype archetype(const CopyProvider<T>& cpy);
    };

    template<class T>
    void ComponentProvider<CopyProvider<T>>::writeComponents(CopyProvider<T> &cpy, CommandBuffer &cmd) {
        cmd.writeComponentCopy(Component<T>::ID(), &cpy.t);
    }

    template<class T>
    Archetype ComponentProvider<CopyProvider<T>>::archetype(const CopyProvider<T>& cpy) {
        return Archetype::create<T>();
    }

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
        Entity* entity = entityPool.getFreeEntity();
        write(CommandType::CreateEntity);
        write(entity);
        write((ComponentProvider<std::remove_reference_t<std::remove_const_t<T>>>::archetype(t) | ...));
        (ComponentProvider<std::remove_reference_t<std::remove_const_t<T>>>::writeComponents(t, *this), ...);
        commandCount++;
        return EntityRef{
            entity,
            entity->version
        };
    }

    template<class T>
    void CommandBuffer::createComponent(const EntityRef &entity, T&& t) {
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
