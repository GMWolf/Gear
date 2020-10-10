//
// Created by felix on 01/04/2020.
//

#ifndef TBTG_RENDERSYSTEM_H
#define TBTG_RENDERSYSTEM_H

#include <gear/shader_generated.h>
#include "g2d.h"

namespace gear {

    class Shader;

    namespace ecs {
        class Registry;
    }

    void renderSprites(G2DInstance* g2d, gear::ecs::Registry &ecs, const gear::assets::Shader* shader);

    void renderDebugShapes(G2DInstance* g2d, gear::ecs::Registry &eCs, const gear::assets::Shader* shader);

}
#endif //TBTG_RENDERSYSTEM_H
