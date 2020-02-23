//
// Created by felix on 08/02/2020.
//

#include <gear/ECS/Archetype.h>

namespace gear::ecs {

    Archetype::Archetype(std::initializer_list<ComponentId> componentIds) {
        for (auto &id : componentIds) {
            bits.set(id);
        }
        bits.set(Component<Entity>::ID());
    }

    bool Archetype::matches(const Archetype &o) const {
        return (bits & o.bits) == o.bits;
    }

    bool Archetype::operator==(const Archetype &o) const {
        return bits == o.bits;
    }

    bool Archetype::operator[](ComponentId id) const{
        return bits[id];
    }

    std::size_t Archetype::Hash::operator()(const gear::ecs::Archetype &a) const noexcept {
        return std::hash<decltype(a.bits)>{}(a.bits);
    }

}