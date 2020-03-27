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
    gear::ecs::Chunk* chunks[1024];
    auto chunkCount = world.queryChunks(gear::ecs::Query().all<gear::CollisionShape, gear::Transform>(), chunks, 1024);

    for(int i = 0; i < chunkCount; i++) {
        auto chunkA = gear::ecs::ChunkView<gear::ecs::Entity, gear::CollisionShape, gear::Transform>(*chunks[i]);

        for(int j = i; j < chunkCount; j++) {
            auto chunkB = gear::ecs::ChunkView<gear::ecs::Entity, gear::CollisionShape, gear::Transform>( *chunks[j]);

            for(auto ea = chunkA.begin(); ea != chunkA.end(); ++ea) {
                for(auto eb = (chunkA == chunkB) ? (ea + 1) : chunkB.begin(); eb != chunkB.end(); ++eb) {
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
