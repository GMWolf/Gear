//
// Created by felix on 01/09/2020.
//

#ifndef GEAR_G2DINSTANCE_H
#define GEAR_G2DINSTANCE_H

#include "Texture.h"
#include <Shader.h>
#include "PrimDraw.h"
#include "FontCache.h"

namespace gear {

    struct G2DInstance {
        SpriteBatch* spriteBatch;
        TextureStore* textureStore;
        ShaderStore* shaderStore;
        PrimDraw* primDraw;
        FontCache* fontCache;
    };

}
#endif //GEAR_G2DINSTANCE_H
