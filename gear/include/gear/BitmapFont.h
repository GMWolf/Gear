//
// Created by felix on 24/02/2020.
//

#ifndef GEAR_BITMAPFONT_H
#define GEAR_BITMAPFONT_H

#include <string>
#include <glm/vec2.hpp>

namespace gear {

    class TextureStore;

    namespace assets {
        class Font;
    }

    class SpriteBatch;
    void renderText(const std::string &text, const gear::assets::Font* font, glm::vec2 pos, SpriteBatch &batch, TextureStore& textureStore);
}

#endif //GEAR_BITMAPFONT_H
