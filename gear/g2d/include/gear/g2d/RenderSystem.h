//
// Created by felix on 01/04/2020.
//

#ifndef TBTG_RENDERSYSTEM_H
#define TBTG_RENDERSYSTEM_H

#include <gear/shader_generated.h>
#include "SpriteBatch.h"
#include "g2d.h"

namespace gear {

    class SpriteBatch;
    class PrimDraw;
    class Shader;
    class TextureStore;

    namespace ecs {
        class Registry;
    }

    void renderSprites(G2DInstance* g2d, gear::ecs::Registry &ecs, const gear::assets::Shader* shader);

    void renderDebugShapes(G2DInstance* g2d, gear::ecs::Registry &eCs, gear::PrimDraw& primDraw, const gear::assets::Shader* shader);

}
#endif //TBTG_RENDERSYSTEM_H
