//
// Created by felix on 07/04/2020.
//

#ifndef GEAR_TILEMAPSYSTEM_H
#define GEAR_TILEMAPSYSTEM_H

#include <gear/AssetManager.h>
#include <cstdint>
#include <glad/glad.h>

namespace gear {

    class TileMap;
    struct TilemapComponent {
        AssetReference<TileMap> tilemap;
        uint16_t chunkWidth;
        uint16_t chunkHeight;
    };

    struct TilemapSystemComponent {
        GLuint vertexBuffer;
        GLuint elementBuffer;
    };

    namespace ecs {
        class Registry;
        class CommandBuffer;
    }

    void TilemapSystemCreateSystemComponent(ecs::Registry& ecs, ecs::CommandBuffer& cmdbuffer);


}


#endif //GEAR_TILEMAPSYSTEM_H
