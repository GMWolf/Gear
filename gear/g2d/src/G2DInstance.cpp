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

    G2DInstance::G2DInstance(const G2DInstanceCreateInfo &createInfo) {
        spriteBatch = new SpriteBatch(*createInfo.spriteBatchCreateInfo);
        shaderStore = new ShaderStore();
        textureStore = new TextureStore();
        primDraw = new PrimDraw();
        fontCache = new FontCache(1024);
    }

    G2DInstance::~G2DInstance() {
        delete spriteBatch;
        delete shaderStore;
        delete textureStore;
        delete primDraw;
        delete fontCache;
    }

    void G2DInstance::flush() {
        spriteBatch->flush();
    }
}