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


static void checkCollisions(gear::ecs::World& world, gear::ecs::CommandBuffer& cmd, CollisionFilter filter) {



    gear::ecs::Chunk* chunksA[1024];
    auto chunkCountA = world.queryChunks(filter.entityA.all<gear::CollisionShape, gear::Transform>(), chunksA, 1024);
    gear::ecs::Chunk* chunksB[1024];
    auto chunkCountB = world.queryChunks(filter.entityB.all<gear::CollisionShape, gear::Transform>(), chunksB, 1024);

    for(int i = 0; i < chunkCountA; i++) {
        auto chunkA = gear::ecs::ChunkView<gear::ecs::Entity, gear::CollisionShape, gear::Transform>(*chunksA[i]);
        auto chunkAEnd = chunkA.end();

        for(int j = 0; j < chunkCountB; j++) {
            auto chunkB = gear::ecs::ChunkView<gear::ecs::Entity, gear::CollisionShape, gear::Transform>( *chunksB[j]);
            auto chunkBEnd = chunkB.end();

            for(auto ea = chunkA.begin(); ea != chunkAEnd; ++ea) {
                for(auto eb = (chunkA == chunkB) ? (ea + 1) : chunkB.begin(); eb != chunkBEnd; ++eb) {
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

void checkCollisions(gear::ecs::World &world, gear::ecs::CommandBuffer &cmd) {

    //Get all filters
    gear::ecs::Chunk* filterChunks[1];
    auto filterChunkCount = world.queryChunks(gear::ecs::Query().all<CollisionFilter>(), filterChunks, 1);
    for(int i = 0; i < filterChunkCount; i++) {
        auto chunk = gear::ecs::ChunkView<CollisionFilter>(*filterChunks[i]);

        for(auto [filter] : chunk) {
            checkCollisions(world, cmd, filter);
        }
    }



}
