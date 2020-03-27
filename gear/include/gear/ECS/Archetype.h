//
// Created by felix on 08/02/2020.
//

#ifndef GEAR_ARCHETYPE_H
#define GEAR_ARCHETYPE_H

#include "Config.h"
#include "Component.h"

#include <initializer_list>
#include <bitset>

namespace gear::ecs {

    class Archetype {
    public:
        Archetype(std::initializer_list<ComponentId> t);
        Archetype() = default;
        template<class... A>
        static Archetype create();

        [[nodiscard]] bool matches(const Archetype &o) const;

        bool operator==(const Archetype& o) const;

        bool operator[](ComponentId id) const;

        struct Hash {
            std::size_t operator()(const gear::ecs::Archetype& a) const noexcept;
        };

        std::bitset<MaxComponents> bits{};
    };


    template<class... A>
    Archetype Archetype::create() {
        return Archetype({Component<A>::ID()...});
    }

    //Entity Component
    struct Entity {
        EntityId id = 0;
        Archetype archetype{};
    };

    struct Query {
        std::bitset<MaxComponents> allBits {};
        std::bitset<MaxComponents> noneBits {};
        std::bitset<MaxComponents> oneBits {};

        template<class... T>
        Query& all() {
            (allBits.set(Component<T>::ID()), ...);
            return *this;
        }

        template<class... T>
        Query& none() {
            (noneBits.set(Component<T>::ID()), ...);
            return *this;
        }

        template<class... T>
        Query& one() {
            (noneBits.set(Component<T>::ID()), ...);
            return *this;
        }
    };

    bool testQuery(const Query& q, const Archetype& a);

}

#endif //GEAR_ARCHETYPE_H
