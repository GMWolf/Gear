//
// Created by felix on 07/04/2020.
//

#ifndef GEAR_TILEMAPSYSTEM_H
#define GEAR_TILEMAPSYSTEM_H

#include <gear/Assets.h>
#include <cstdint>
#include <gear/map_generated.h>
#include "g2d.h"

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
    void tilemapSystemRender(G2DInstance& g2d, ecs::Registry& ecs, const assets::Shader* shader);
}


#endif //GEAR_TILEMAPSYSTEM_H
