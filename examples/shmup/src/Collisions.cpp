//
// Created by felix on 13/02/2020.
//

#include "Collisions.h"

bool CollisionPair::matchesPair(const gear::ecs::Archetype &a, const gear::ecs::Archetype &b) {
    return (entity[0].archetype.matches(a) && entity[1].archetype.matches(b));
}

std::optional<std::pair<gear::ecs::Entity, gear::ecs::Entity>>
CollisionPair::get(const gear::ecs::Archetype &a, const gear::ecs::Archetype &b) {
    if (matchesPair(a, b)) return {{entity[0], entity[1]}};
    if (matchesPair(b, a)) return {{entity[1], entity[0]}};
    return {};
}

void checkCollisions(gear::ecs::World &world, gear::ecs::CommandBuffer &cmd) {

    auto chunks = world.getChunks<gear::ecs::Entity, gear::CollisionShape, gear::Transform>();

    for(auto itA = chunks.begin(); itA != chunks.end(); ++itA) {
        for(auto itB = itA; itB != chunks.end(); ++itB) {

            //Chunk View end is expensive, apparently. best to move it out here
            auto itAend = (*itA).end();
            auto itBend = (*itB).end();

            for(auto ea = (*itA).begin(); ea != itAend; ea++) {
                for(auto eb = (itA == itB) ? (ea + 1) : (*itB).begin(); eb != itBend; eb++) {

                    auto [entityA, shapeA, transformA] = *ea;
                    auto [entityB, shapeB, transformB] = *eb;

                    if (gear::collide(shapeA, transformA.pos, shapeB, transformB.pos)) {
                        cmd.createEntity(CollisionPair{entityA, entityB});
                    }
                }
            }
        }
    }


    /*
      for(auto itA = chunks.begin(); itA != chunks.end(); ++itA) {
        for(auto itB = itA; itB != chunks.end(); ++itB) {

            for(auto ea = (*itA).begin(); ea != (*itA).end(); ea++) {
                for(auto eb = (itA == itB) ? (ea + 1) : (*itB).begin(); eb != (*itB).end(); eb++) {

                    auto [entityA, shapeA, transformA] = *ea;
                    auto [entityB, shapeB, transformB] = *eb;

                    if (gear::collide(shapeA, transformA.pos, shapeB, transformB.pos)) {
                        cmd.createEntity(CollisionPair{entityA, entityB});
                    }
                }
            }
        }
    }
     */
}
