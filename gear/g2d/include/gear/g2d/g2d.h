//
// Created by felix on 01/09/2020.
//

#ifndef GEAR_G2D_H
#define GEAR_G2D_H

#include <optional>

namespace gear {

    struct G2DInstance;
    struct SpriteBatchCreateInfo;

    struct G2DInstanceCreateInfo {
        SpriteBatchCreateInfo* spriteBatchCreateInfo;
    };

    G2DInstance* createG2DInstance(const G2DInstanceCreateInfo& createInfo);
    void destroyG2DInstance(G2DInstance* instance);
}

#endif //GEAR_G2D_H
