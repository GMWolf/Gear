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
    private:
        std::bitset<MaxComponents> bits{};
    };


    template<class... A>
    Archetype Archetype::create() {
        return Archetype({Component<A>::ID()...});
    }

}

#endif //GEAR_ARCHETYPE_H
