//
// Created by felix on 07/04/2020.
//

#include <gear/map/TilemapSystem.h>
#include <gear/map/TileMap.h>
#include <gear/ECS/ECS.h>

namespace gecs = gear::ecs;

void gear::TilemapSystemCreateSystemComponent(ecs::Registry &ecs, gecs::CommandBuffer& cmdbuffer) {

    gecs::CommandEncoder cmd(cmdbuffer);

    gecs::Chunk* chunkArray[512];
    auto chunks = ecs.queryChunks(gecs::Query().all<TilemapComponent>().none<TilemapSystemComponent>(), chunkArray, 512);


    for(auto c : chunks) {
        auto chunk = gecs::ChunkView<gecs::EntityRef>(*c);

        for(auto [entity] : chunk)  {

        }

    }

}