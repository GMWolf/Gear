//
// Created by felix on 04/09/2020.
//

#ifndef GEAR_FONTCACHE_H
#define GEAR_FONTCACHE_H

#include <unordered_map>
#include <gear/font_generated.h>
#include "Texture.h"
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace gear {

    struct GlyphEntry {
        const assets::Font* font;
        uint32_t glyphIndex;
        uint16_t pixelWidth;
        uint16_t pixelHeight;
    };

    struct GlyphEntryHash {
        std::size_t operator()(const GlyphEntry& g) const;
    };

    bool operator==(const GlyphEntry& a, const GlyphEntry& b);

    struct GlyphUvs {
        uint16_t xoffset, yoffset;
        uint16_t width, height;
    };

    struct FontCache {
        std::unordered_map<GlyphEntry, GlyphUvs, GlyphEntryHash> glyphs;
        std::unordered_map<const assets::Font*, FT_Face> faces;
        std::optional<gear::Texture> texture;
        size_t texSize;
        FT_Library ftLibrary;

        void addGlyph(const GlyphEntry &glyphEntry);

        explicit FontCache(size_t texSize);
    };

}


#endif //GEAR_FONTCACHE_H
