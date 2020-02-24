//
// Created by felix on 24/02/2020.
//

#ifndef GEAR_BITMAPFONT_H
#define GEAR_BITMAPFONT_H

#include <string>
#include <memory>
#include "Texture.h"
#include <glm/vec4.hpp>
#include <vector>

namespace gear {

    class BitmapFont {
    public:
        struct Glyph {
            glm::vec4 uv;
            glm::vec2 size;
            float advance;
            glm::vec2 offset;
        };

        explicit BitmapFont(const std::string& name);

        Glyph operator[](char c) const;


        std::unique_ptr<Texture> texture;
    private:
        int rangeStart {0};
        int rangeCount {0};
        std::vector<Glyph> glyphs;
    };


    class SpriteBatch;
    void renderText(const std::string& text, const BitmapFont& font, glm::vec2 pos, SpriteBatch& batch);
}

#endif //GEAR_BITMAPFONT_H
