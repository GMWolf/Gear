//
// Created by felix on 01/09/2020.
//

#include <g2d.h>
#include "G2DInstance.h"

namespace gear {


    G2DInstance *createG2DInstance(G2DInstanceCreateInfo &createInfo) {
        auto* g2dInstance = new G2DInstance();

        g2dInstance->spriteBatch = createSpriteBatch(*createInfo.spriteBatchCreateInfo);
        g2dInstance->shaderStore = new ShaderStore();
        g2dInstance->textureStore = new TextureStore();

        return g2dInstance;
    }

    void destroyG2DInstance(G2DInstance *instance) {
        destroySpriteBatch(instance->spriteBatch);
        delete instance->shaderStore;
        delete instance->textureStore;
        delete instance;
    }

}