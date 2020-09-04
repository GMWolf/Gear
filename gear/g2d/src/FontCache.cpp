//
// Created by felix on 04/09/2020.
//

#include "FontCache.h"
#include <gear/Util.h>
#include <stb_truetype.h>

std::size_t gear::GlyphEntryHash::operator()(const gear::GlyphEntry &g) {
    size_t result = std::hash<const assets::Font *>{}(g.font);
    result = hashCombine(result, std::hash<uint32_t>{}(g.glyphIndex));
    result = hashCombine(result, std::hash<uint16_t>{}(g.pixelWidth));
    result = hashCombine(result, std::hash<uint16_t>{}(g.pixelHeight));
    return result;
}

gear::FontCache::FontCache(size_t texSize) : texSize(texSize) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, texSize, texSize, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void gear::FontCache::addGlyph(const gear::GlyphEntry& glyphEntry) {
    if (glyphs.find(glyphEntry) == glyphs.end()) {



    }
}
