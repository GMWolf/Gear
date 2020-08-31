//
// Created by felix on 07/04/2020.
//

#ifndef GEAR_TILEMAPSYSTEM_H
#define GEAR_TILEMAPSYSTEM_H

#include <gear/AssetManager.h>
#include <cstdint>
#include <glad/glad.h>
#include <generated/map_generated.h>

namespace gear {

    struct TilemapComponent {
        const gear::assets::TileSet* tileSet;
        uint16_t width, height;
        const uint32_t* data;
    };

    namespace ecs {
        class Registry;
        class CommandBuffer;
    }

    void tilemapSystemCreateSystemComponent(ecs::Registry& ecs, ecs::CommandBuffer& cmdbuffer);


    class Shader;
    class TextureStore;
    void tilemapSystemRender(ecs::Registry& ecs, const Shader &shader, TextureStore& textureStore);
}


#endif //GEAR_TILEMAPSYSTEM_H
