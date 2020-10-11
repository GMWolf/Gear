//
// Created by felix on 01/04/2020.
//

#ifndef TBTG_RENDERSYSTEM_H
#define TBTG_RENDERSYSTEM_H

#include <gear/shader_generated.h>
#include <gear/sprite_generated.h>
#include <gear/bitmapFont_generated.h>
#include "g2d.h"

namespace gear {

    class Shader;

    namespace ecs {
        class Registry;
        class CommandBuffer;
    }

    struct SpriteComponent {
        const assets::Sprite* sprite{};
        uint16_t imageIndex{0};
    };

    struct DestroyOnAnimationEnd {
    };

    struct Text {
        std::string text;
        const gear::assets::BitmapFont* font;
    };

    void renderSprites(G2DInstance* g2d, gear::ecs::Registry &ecs, const gear::assets::Shader* shader, gear::ecs::CommandBuffer& cmd);

    void renderDebugShapes(G2DInstance* g2d, gear::ecs::Registry &eCs, const gear::assets::Shader* shader);

    void renderTextSystem(G2DInstance* g2d, gear::ecs::Registry& ecs, const gear::assets::Shader* shader);

}
#endif //TBTG_RENDERSYSTEM_H
