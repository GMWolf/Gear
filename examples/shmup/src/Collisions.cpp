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

    auto chunks = world.getChunks(gear::ecs::Archetype::create<gear::ecs::Entity, gear::CollisionShape, gear::Transform>());

    int loopCounter = 0;
    for(auto itA = chunks.begin(); itA != chunks.end(); ++itA) {

        auto chunkA = gear::ecs::ChunkView<gear::ecs::Entity, gear::CollisionShape, gear::Transform>(*itA);
        auto itAend = chunkA.end();
        for(auto itB = itA; itB != chunks.end(); ++itB) {
            auto chunkB = gear::ecs::ChunkView<gear::ecs::Entity, gear::CollisionShape, gear::Transform>(*itB);
            auto itBend = chunkB.end();
            loopCounter+=1;

            for(auto ea = chunkA.begin(); ea != itAend; ea++) {
                for(auto eb = (itA == itB) ? (ea + 1) : chunkB.begin(); eb != itBend; eb++) {

                    auto [entityA, shapeA, transformA] = *ea;
                    auto [entityB, shapeB, transformB] = *eb;

                    if (gear::collide(shapeA, transformA.pos, shapeB, transformB.pos)) {
                        cmd.createEntity(CollisionPair{entityA, entityB});
                    }
                }
            }
        }
    }
}
