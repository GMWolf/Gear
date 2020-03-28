//
// Created by felix on 28/03/2020.
//

#ifndef GEAR_COMMANDBUFFER_H
#define GEAR_COMMANDBUFFER_H


#include "Archetype.h"
#include <memory>

namespace gear::ecs {

    enum class CommandType {
        CreateEntity,
        DestroyEntity,
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

        bool writeComponent(ComponentId id, void *ptr);

    public:
        explicit CommandEncoder(CommandBuffer &cmd) : cmd(cmd), head(cmd.buffer), space(cmd.bufferSize) {};

        template<class... T>
        void createEntity(T&&... t);
        void destroyEntity(Entity entity);

        void reset();
    };

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
        write(Archetype::create<std::remove_reference_t<T>...>());
        (writeComponent(Component<std::remove_reference_t<T>>::ID(), &t), ...);
        cmd.commandCount++;
    }

    class World;

    void executeCommandBuffer(CommandBuffer& cmd, World& world);

    void resetCommandBuffer(CommandBuffer& buffer);

}

#endif //GEAR_COMMANDBUFFER_H
