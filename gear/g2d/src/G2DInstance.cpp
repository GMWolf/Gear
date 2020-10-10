//
// Created by felix on 01/09/2020.
//

#include <g2d.h>
#include "SpriteBatch.h"
#include "Shader.h"
#include "Texture.h"
#include "PrimDraw.h"
#include "FontCache.h"

namespace gear {

    G2DInstance *createG2DInstance(const G2DInstanceCreateInfo &createInfo) {
        auto* g2dInstance = new G2DInstance();

        g2dInstance->spriteBatch = new SpriteBatch(*createInfo.spriteBatchCreateInfo);
        g2dInstance->shaderStore = new ShaderStore();
        g2dInstance->textureStore = new TextureStore();
        g2dInstance->primDraw = new PrimDraw();
        g2dInstance->fontCache = new FontCache(1024);

        return g2dInstance;
    }

    void destroyG2DInstance(G2DInstance *instance) {
        delete instance->spriteBatch;
        delete instance->shaderStore;
        delete instance->textureStore;
        delete instance->primDraw;
        delete instance->fontCache;
        delete instance;
    }

    void G2DInstance::flush() {
        spriteBatch->flush();
    }
}