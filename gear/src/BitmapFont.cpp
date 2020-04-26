//
// Created by felix on 24/02/2020.
//

#include <gear/BitmapFont.h>
#include <fstream>
#include <gear/SpriteBatch.h>
#include <generated/font_generated.h>

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

gear::BitmapFont gear::BitmapFontLoader::load(const std::string &name, AssetRegistry& registry) {


    std::ifstream in(name, std::ios::binary);
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    in.seekg(0, std::ios::beg);
    auto buffer = (char*)malloc(size);
    in.read(buffer, size);

    auto fontBin = gear::bin::GetFont(buffer);

    BitmapFont font;
    registry.load<Texture>(fontBin->texture()->str());
    font.texture = registry.get<Texture>(fontBin->texture()->str());

    font.rangeStart = fontBin->range_start();
    font.rangeCount = fontBin->range_count();

    font.glyphs.reserve(font.rangeCount);

    for(auto g : *fontBin->glyphs()) {
        BitmapFont::Glyph glyph {};
        glyph.uv.x = g->x0();
        glyph.uv.y = g->y1();
        glyph.uv.z = g->x1();
        glyph.uv.w = g->y0();
        glyph.size = {glyph.uv.z - glyph.uv.x, glyph.uv.y - glyph.uv.w};
        glyph.uv /= glm::vec4{font.texture->size, font.texture->size};
        glyph.advance = g->xadvance();
        glyph.offset.x = g->xoff();
        glyph.offset.y = g->yoff2();

        font.glyphs.push_back(glyph);
    }

    free(buffer);
    return font;
}
