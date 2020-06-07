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

    struct CommandBuffer {
        explicit CommandBuffer(size_t size);
        std::byte* buffer {nullptr};
        size_t bufferSize {0};
        size_t commandCount {0};
    };

    class CommandEncoder {
        CommandBuffer& cmd;
        void *head;
        size_t space;

    private:
        void *allocate(size_t size, size_t align);

        template<class T>
        T *allocate();

        template<class T>
        bool write(const T& t);

    public:
        explicit CommandEncoder(CommandBuffer &cmd) : cmd(cmd), head(cmd.buffer), space(cmd.bufferSize) {};

        template<class... T>
        void createEntity(T&&... t);
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
        static void writeComponents(T& t, CommandEncoder& encoder);
        static Archetype archetype(const T& t);
    };

    template<class T>
    Archetype ComponentProvider<T>::archetype(const T& t) {
        return Archetype::create<T>();
    }

    template<class T>
    void ComponentProvider<T>::writeComponents(T& t, CommandEncoder& encoder){
        encoder.writeComponentMove(Component<T>::ID(), &t);
    }

    template<class T>
    struct CopyProvider {
            explicit CopyProvider(T& t);
            const T& t;
    };

    template<class T>
    CopyProvider<T>::CopyProvider(T &t) : t(t){
    }

    template<class T>
    struct ComponentProvider<CopyProvider<T>> {
        static void writeComponents(CopyProvider<T>& cpy, CommandEncoder& encoder);
        static Archetype archetype(const CopyProvider<T>& cpy);
    };


    template<class T>
    Archetype ComponentProvider<CopyProvider<T>>::archetype(const CopyProvider<T>& cpy) {
        return Archetype::create<T>();
    }

    template<class T>
    void ComponentProvider<CopyProvider<T>>::writeComponents(CopyProvider<T>& cpy, CommandEncoder& encoder){
        encoder.writeComponentCopy(Component<T>::ID(), &cpy.t);
    }

    template<class T>
    T *CommandEncoder::allocate() {
        return reinterpret_cast<T *>(allocate(sizeof(T), alignof(T)));
    }

    template<class T>
    bool CommandEncoder::write(const T& t) {
        if (T* ptr = allocate<T>()) {
            *ptr = t;
            return true;
        } else {
            return false;
        }
    }

    template<class... T>
    void CommandEncoder::createEntity(T &&... t) {
        write(CommandType::CreateEntity);
        write((ComponentProvider<std::remove_reference_t<T>>::archetype(t) | ...));
        (ComponentProvider<std::remove_reference_t<T>>::writeComponents(t, *this), ...);
        cmd.commandCount++;
    }

    template<class T>
    void CommandEncoder::createComponent(const EntityRef &entity, T&& t) {
        write(CommandType::CreateComponent);
        write(entity);
        writeComponentMove(Component<std::remove_reference_t<T>>::ID(), &t);
        cmd.commandCount++;
    }

    template<class T>
    void CommandEncoder::destroyComponent(const EntityRef &entity) {
        write(CommandType::DestroyComponent);
        write(entity);
        write(Component<T>::ID());
        cmd.commandCount++;
    }

    class Registry;

    void executeCommandBuffer(CommandBuffer& cmd, Registry& world);

    void resetCommandBuffer(CommandBuffer& buffer);

}

#endif //GEAR_COMMANDBUFFER_H
