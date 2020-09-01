//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_SPRITEBATCH_H
#define GEAR_SPRITEBATCH_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Sprite.h"

namespace gear {

    class Texture;

    class SpriteBatch;

    struct SpriteBatchCreateInfo {
        size_t batchSize;
    };

    SpriteBatch * createSpriteBatch(const SpriteBatchCreateInfo& createInfo);
    void destroySpriteBatch(SpriteBatch*);

    void spriteBatchDraw(SpriteBatch& spriteBatch, const gear::Texture& tex, glm::vec2 pos, glm::vec2 size, glm::vec4 uv);
    void spriteBatchDraw(SpriteBatch& spriteBatch, const gear::Texture& tex, const gear::TexRegion& texRegion, glm::vec2 pos, glm::vec2 size);
    void spriteBatchFlush(SpriteBatch& spriteBatch);
}

#endif //GEAR_SPRITEBATCH_H
