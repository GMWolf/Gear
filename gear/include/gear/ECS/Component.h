//
// Created by felix on 08/02/2020.
//

#ifndef GEAR_COMPONENT_H
#define GEAR_COMPONENT_H

#include <cstddef>
#include <utility>
#include "Config.h"

namespace gear::ecs {
    extern ComponentId nextComponentId;

    struct ComponentInfo {
        size_t size{0};
        size_t align{0};
        ComponentId id{0};

        struct {
            void (*emplace)(void* ptr, void* from);
            void (*destroy)(void* ptr);
            void (*move)(void* from, void* to);
        } functions;

        static ComponentInfo component[MaxComponents];
    };

    template<class T>
    struct Component {
        static ComponentInfo info();
        static ComponentId ID();

        struct Functions {
            static void emplace(void* ptr, void* from);
            static void destroy(void* ptr);
            static void move(void* from, void* to);
        };

    private:
        static ComponentInfo createInfo();
    };

    template<class T>
    ComponentInfo Component<T>::createInfo() {
        ComponentInfo i;
        i.size = sizeof(T);
        i.align = alignof(T);
        i.id = nextComponentId++;
        i.functions.emplace = &Functions::emplace;
        i.functions.destroy = &Functions::destroy;
        i.functions.move = &Functions::move;

        ComponentInfo::component[i.id] = i;

        return i;
    }

    template<class T>
    ComponentInfo Component<T>::info() {
        static ComponentInfo info = createInfo();
        return info;
    }

    template<class T>
    ComponentId Component<T>::ID() {
        return info().id;
    }

    template<class T>
    void Component<T>::Functions::emplace(void *ptr, void *from) {
        T& t = *static_cast<T*>(from);
        new (ptr) T(std::move(t));
    }

    template<class T>
    void Component<T>::Functions::destroy(void *ptr) {
        T& t = *static_cast<T*>(ptr);
        t.~T();
    }

    template<class T>
    void Component<T>::Functions::move(void *from, void *to) {
        T& tFrom = *static_cast<T*>(from);
        T& tTo = *static_cast<T*>(to);
        tTo = std::move(tFrom);
    }

}


#endif //GEAR_COMPONENT_H
