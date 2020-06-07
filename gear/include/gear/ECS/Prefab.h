//
// Created by felix on 07/06/2020.
//

#ifndef GEAR_PREFAB_H
#define GEAR_PREFAB_H

#include "Core.h"
#include "CommandBuffer.h"

namespace gear::ecs {

    struct Prefab {
        EntityRef entity{};
    };

    template<>
    struct ComponentProvider<gear::ecs::Prefab> {
        static void writeComponents(Prefab& prefab, CommandEncoder& encoder);
        static Archetype archetype(const Prefab& prefab);
    };

    inline void ComponentProvider<Prefab>::writeComponents(Prefab &prefab, CommandEncoder &encoder) {
        if (prefab.entity.alive()) {
            auto a = archetype(prefab);
            for(int i = 0; i < a.bits.size(); i++) {
                if (a[i]) {
                    encoder.writeComponentCopy(i, prefab.entity.entity->chunk->get(i, prefab.entity.entity->index));
                }
            }
        }
    }

    inline Archetype ComponentProvider<Prefab>::archetype(const Prefab &prefab) {
        if (prefab.entity.alive()) {
            return prefab.entity.entity->chunk->archetype / Component<EntityRef>::ID();
        } else {
            return Archetype();
        }
    }

}

#endif //GEAR_PREFAB_H
