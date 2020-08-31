//
// Created by felix on 24/02/2020.
//

#ifndef GEAR_BITMAPFONT_H
#define GEAR_BITMAPFONT_H

#include <string>
#include <memory>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include "AssetManager.h"

namespace gear {

    class Texture;

    class BitmapFont {
    public:
        struct Glyph {
            glm::vec4 uv;
            glm::vec2 size;
            float advance;
            glm::vec2 offset;
        };

        Glyph operator[](char c) const;

        Texture** texture;
    private:
        int rangeStart {0};
        int rangeCount {0};
        std::vector<Glyph> glyphs;
        friend class BitmapFontLoader;
    };

    namespace assets {
        class Font;
    }

    class BitmapFontLoader {
    public:
        static BitmapFont load(const assets::Font* fontDef, AssetRegistry& registry);
    };

    class SpriteBatch;
    void renderText(const std::string& text, const BitmapFont& font, glm::vec2 pos, SpriteBatch& batch);
}

#endif //GEAR_BITMAPFONT_H
