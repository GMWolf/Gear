//
// Created by felix on 24/02/2020.
//

#include <gear/BitmapFont.h>
#include <fstream>
#include <gear/SpriteBatch.h>
#include <generated/font_generated.h>
#include <gear/Texture.h>

gear::BitmapFont::Glyph gear::BitmapFont::operator[](char c) const {
    return glyphs[c - rangeStart];
}

void gear::renderText(const std::string &text, const BitmapFont &font, glm::vec2 pos, SpriteBatch &batch) {

    for(char c: text) {
        auto glyph = font[c];
        batch.draw(*font.texture, pos + glyph.offset, glyph.size, glyph.uv);
        pos.x += glyph.advance;
    }
}

gear::BitmapFont gear::BitmapFontLoader::load(const gear::assets::Font *fontDef, gear::AssetRegistry &registry) {
    BitmapFont font;

    auto texName = fontDef->texture()->str();
    font.texture = registry.getTexture(texName);

    font.rangeStart = fontDef->range_start();
    font.rangeCount = fontDef->range_count();

    font.glyphs.reserve(font.rangeCount);

    for(auto g : *fontDef->glyphs()) {
        BitmapFont::Glyph glyph {};
        glyph.uv.x = g->x0();
        glyph.uv.y = g->y0();
        glyph.uv.z = g->x1();
        glyph.uv.w = g->y1();
        glyph.size = {glyph.uv.z - glyph.uv.x, glyph.uv.w - glyph.uv.y};
        glyph.uv /= glm::vec4{font.texture->size, font.texture->size};
        glyph.advance = g->xadvance();
        glyph.offset.x = g->xoff();
        glyph.offset.y = g->yoff2();

        font.glyphs.push_back(glyph);
    }

    return font;
}
