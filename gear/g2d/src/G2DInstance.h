//
// Created by felix on 01/09/2020.
//

#ifndef GEAR_G2DINSTANCE_H
#define GEAR_G2DINSTANCE_H

#include "Texture.h"
#include <Shader.h>

namespace gear {

    struct G2DInstance {
        SpriteBatch *spriteBatch;
        TextureStore *textureStore;
        ShaderStore *shaderStore;
    };

}
#endif //GEAR_G2DINSTANCE_H