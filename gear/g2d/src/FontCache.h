//
// Created by felix on 04/09/2020.
//

#ifndef GEAR_FONTCACHE_H
#define GEAR_FONTCACHE_H

#include <unordered_map>
#include <gear/font_generated.h>
#include "Texture.h"
#include <vector>

namespace gear {

    struct GlyphEntry {
        const assets::Font* font;
        uint32_t glyphIndex;
        uint16_t pixelWidth;
        uint16_t pixelHeight;
    };

    struct GlyphEntryHash {
        std::size_t operator()(const GlyphEntry& g);
    };

    struct GlyphUvs {
        uint16_t xoffset, yoffset;
        uint16_t width, height;
    };

    struct FontCache {
        std::unordered_map<GlyphEntry, GlyphUvs, GlyphEntryHash> glyphs;
        GLuint tex {0};
        size_t texSize;

        void addGlyph(const GlyphEntry& glyphEntry);


        explicit FontCache(size_t texSize);
    };

}


#endif //GEAR_FONTCACHE_H
