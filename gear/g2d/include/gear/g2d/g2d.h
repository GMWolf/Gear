//
// Created by felix on 01/09/2020.
//

#ifndef GEAR_G2D_H
#define GEAR_G2D_H

#include <cstddef>

namespace gear {

    struct G2DInstance;
    struct SpriteBatchCreateInfo;
    struct SpriteBatch;
    struct TextureStore;
    struct ShaderStore;
    struct PrimDraw;
    struct FontCache;

    struct G2DInstanceCreateInfo {
        SpriteBatchCreateInfo* spriteBatchCreateInfo;
    };

    struct SpriteBatchCreateInfo {
        size_t batchSize;
    };

    struct G2DInstance {

        explicit G2DInstance(const G2DInstanceCreateInfo &createInfo);
        ~G2DInstance();

        SpriteBatch* spriteBatch;
        TextureStore* textureStore;
        ShaderStore* shaderStore;
        PrimDraw* primDraw;
        FontCache* fontCache;

        void flush();
    };

}

#endif //GEAR_G2D_H
