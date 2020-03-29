//
// Created by felix on 13/02/2020.
//

#include "Collisions.h"

bool CollisionPair::matchesPair(const gear::ecs::Archetype &a, const gear::ecs::Archetype &b) {
    return alive() && (entity[0].entity->chunk->archetype.matches(a) && entity[1].entity->chunk->archetype.matches(b));
}

std::optional<std::pair<gear::ecs::EntityRef&, gear::ecs::EntityRef&>>
CollisionPair::get(const gear::ecs::Archetype &a, const gear::ecs::Archetype &b) {
    if (matchesPair(a, b)) return {{entity[0], entity[1]}};
    if (matchesPair(b, a)) return {{entity[1], entity[0]}};
    return {};
}


static void checkCollisions(gear::ecs::Registry& ecs, gear::ecs::CommandEncoder& cmd, CollisionFilter filter) {

    gear::ecs::Chunk* chunksAArray[1024];
    auto chunksA = ecs.queryChunks(filter.entityA.all<gear::CollisionShape, gear::Transform>(), chunksAArray, 1024);
    gear::ecs::Chunk* chunksBArray[1024];
    auto chunksB = ecs.queryChunks(filter.entityB.all<gear::CollisionShape, gear::Transform>(), chunksBArray, 1024);

    for(auto ca : chunksA) {
        auto chunkA = gear::ecs::ChunkView<gear::ecs::EntityRef, gear::CollisionShape, gear::Transform>(*ca);
        auto chunkAEnd = chunkA.end();

        for(auto cb : chunksB) {
            auto chunkB = gear::ecs::ChunkView<gear::ecs::EntityRef, gear::CollisionShape, gear::Transform>(*cb);
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

void checkCollisions(gear::ecs::Registry &ecs, gear::ecs::CommandEncoder &cmd) {

    //Get all filters
    gear::ecs::Chunk* filterChunksArray[1];
    auto filterChunks = ecs.queryChunks(gear::ecs::Query().all<CollisionFilter>(), filterChunksArray, 1);
    for(auto c : filterChunks) {
        auto chunk = gear::ecs::ChunkView<CollisionFilter>(*c);

        for(auto [filter] : chunk) {
            checkCollisions(ecs, cmd, filter);
        }
    }



}
