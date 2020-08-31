//
// Created by felix on 01/04/2020.
//

#ifndef TBTG_RENDERSYSTEM_H
#define TBTG_RENDERSYSTEM_H

namespace gear {

    class SpriteBatch;
    class PrimDraw;
    class Shader;
    class TextureStore;
    namespace ecs {
        class Registry;
    }

    void renderSprites(gear::ecs::Registry &ecs, gear::SpriteBatch &batch, const gear::Shader &shader, TextureStore& textureStore);

    void renderDebugShapes(gear::ecs::Registry &evs, gear::PrimDraw& primDraw, const gear::Shader &shader);

}
#endif //TBTG_RENDERSYSTEM_H
