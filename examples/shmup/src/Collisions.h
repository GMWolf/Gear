//
// Created by felix on 13/02/2020.
//

#ifndef GEAR_COLLISIONS_H
#define GEAR_COLLISIONS_H

#include <gear/ecs/ECS.h>
#include <tuple>
#include <gear/CollisionShape.h>
#include <gear/Transform.h>
#include <gear/CollisionDetection.h>
#include <optional>

struct CollisionPair {
    gear::ecs::EntityRef entity[2];

    bool matchesPair(const gear::ecs::Archetype& a, const gear::ecs::Archetype& b);
    bool alive() {
        return entity[0].alive() && entity[1].alive();
    }
    std::optional<std::pair<gear::ecs::EntityRef&, gear::ecs::EntityRef&>> get(const gear::ecs::Archetype& a, const gear::ecs::Archetype& b);
};

struct CollisionFilter {
    gear::ecs::Query entityA;
    gear::ecs::Query entityB;
    std::vector<CollisionPair> collisionPairs;
};

void checkCollisions(gear::ecs::Registry& ecs, CollisionFilter& filter);

#endif //GEAR_COLLISIONS_H
