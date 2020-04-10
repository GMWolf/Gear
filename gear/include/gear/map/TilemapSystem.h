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
    };

    struct TilemapSystemComponent {
        size_t count;
        GLuint vertexArray;
        union {
            struct {
                GLuint vertexBuffer;
                GLuint elementBuffer;
            };
            GLuint buffers[2];
        };
    };

    namespace ecs {
        class Registry;
        class CommandBuffer;
    }

    void tilemapSystemCreateSystemComponent(ecs::Registry& ecs, ecs::CommandBuffer& cmdbuffer);


    class Shader;
    void tilemapSystemRender(ecs::Registry& ecs, const Shader &shader);
}


#endif //GEAR_TILEMAPSYSTEM_H
