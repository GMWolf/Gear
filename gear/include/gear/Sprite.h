//
// Created by felix on 07/01/2020.
//

#ifndef GEAR_SPRITE_H
#define GEAR_SPRITE_H

#include <glm/vec2.hpp>
#include <memory>

namespace gear {

    class Texture;
    struct Sprite {
        glm::vec2 pos;
        glm::vec2 size;
        std::weak_ptr<const Texture> tex;
    };

}

#endif //GEAR_SPRITE_H
