//
// Created by felix on 24/02/2020.
//

#ifndef GEAR_TEXT_H
#define GEAR_TEXT_H

#include <string>
#include <glm/vec2.hpp>
#include "SpriteBatch.h"
#include "g2d.h"
#include <gear/shader_generated.h>
#include <gear/View.h>

namespace gear {

    class TextureStore;

    namespace assets {
        class Font;
    }

    class SpriteBatch;
    void renderText(G2DInstance* g2d, const std::string &text, const gear::assets::Font* font, glm::vec2 pos, const gear::assets::Shader* shader, gear::View view);
}

#endif //GEAR_TEXT_H
