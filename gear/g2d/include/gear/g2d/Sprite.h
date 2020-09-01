//
// Created by felix on 01/09/2020.
//

#ifndef GEAR_SPRITE_H
#define GEAR_SPRITE_H

#include "g2d.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <gear/texture_generated.h>
#include <gear/sprite_generated.h>
#include <gear/CollisionShape.h>


namespace gear {

    struct TexRegion {
        glm::vec4 uvs; // min uv, max uv
        glm::vec4 crop; //left, top, right, bottom
    };

    struct Sprite {
        glm::vec2 size{};
        glm::vec2 origin{};
        std::vector <TexRegion> texRegions{};
        const assets::Texture *tex;
        uint16_t imageIndex{0};

        std::optional <CollisionShape> mask;
    };

    Sprite createSprite(const assets::Sprite* spriteDef);

}

#endif //GEAR_SPRITE_H
