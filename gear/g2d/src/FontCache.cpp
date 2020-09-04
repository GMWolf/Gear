//
// Created by felix on 04/09/2020.
//

#include "FontCache.h"
#include <gear/Util.h>

std::size_t gear::GlyphEntryHash::operator()(const gear::GlyphEntry &g) const{
    size_t result = std::hash<const assets::Font *>{}(g.font);
    result = hashCombine(result, std::hash<uint32_t>{}(g.glyphIndex));
    result = hashCombine(result, std::hash<uint16_t>{}(g.pixelWidth));
    result = hashCombine(result, std::hash<uint16_t>{}(g.pixelHeight));
    return result;
}

gear::FontCache::FontCache(size_t texSize) : texSize(texSize) {

    FT_Init_FreeType(&ftLibrary);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glObjectLabel(GL_TEXTURE, tex, -1, "fontcache");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    char* b = (char*)malloc(texSize * texSize);
    memset(b, 0, texSize * texSize);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, texSize, texSize, 0, GL_RED, GL_UNSIGNED_BYTE, b);
    free(b);
    glBindTexture(GL_TEXTURE_2D, 0);
    texture.emplace(tex, glm::ivec2 {texSize, texSize});
}


void gear::FontCache::addGlyph(const gear::GlyphEntry& glyphEntry) {
    if (glyphs.find(glyphEntry) == glyphs.end()) {

        auto [faceIt, isNewFace] = faces.emplace();
        auto& [key, face] = *faceIt;
        if (isNewFace) {
            auto error = FT_New_Memory_Face(ftLibrary, glyphEntry.font->data()->data(), glyphEntry.font->data()->size(),0,
                               &face);
            assert(!error);
        }

        auto error = FT_Set_Pixel_Sizes(face, glyphEntry.pixelWidth, glyphEntry.pixelHeight);
        assert(!error);
        error = FT_Load_Char(face, glyphEntry.glyphIndex, FT_LOAD_RENDER);
        assert(!error);
        //FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL);
        glBindTexture(GL_TEXTURE_2D, texture->tex);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                        GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

bool gear::operator==(const gear::GlyphEntry &a, const gear::GlyphEntry &b) {
    return a.font == b.font
    && a.glyphIndex == b.glyphIndex
    && a.pixelHeight == b.pixelHeight
    && a.pixelWidth == b.pixelWidth;
}
