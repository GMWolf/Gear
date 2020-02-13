//
// Created by felix on 13/02/2020.
//

#ifndef GEAR_COLLISIONS_H
#define GEAR_COLLISIONS_H

#include <gear/ECS/ECS.h>
#include <tuple>
#include <gear/CollisionShape.h>
#include <gear/CoreComponents.h>
#include <gear/CollisionDetection.h>
#include <optional>

struct CollisionPair {
    gear::ecs::Entity entity[2];

    bool matchesPair(const gear::ecs::Archetype& a, const gear::ecs::Archetype& b);
    std::optional<std::pair<gear::ecs::Entity, gear::ecs::Entity>> get(const gear::ecs::Archetype& a, const gear::ecs::Archetype& b);
};

void checkCollisions(gear::ecs::World& world, gear::ecs::CommandBuffer& cmd);

#endif //GEAR_COLLISIONS_H
