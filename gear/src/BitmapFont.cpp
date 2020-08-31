//
// Created by felix on 24/02/2020.
//

#include <gear/BitmapFont.h>
#include <gear/SpriteBatch.h>
#include <generated/font_generated.h>
#include <gear/Texture.h>


void gear::renderText(const std::string &text, const gear::assets::Font* font, glm::vec2 pos, SpriteBatch &batch, TextureStore& textureStore) {

    auto texture = textureStore.getTexture(font->texture()->ptr());

    for(char c: text) {
        auto glyph = font->glyphs()->Get(c - font->range_start());
        glm::vec2 off = {glyph->xoff(), glyph->yoff2()};
        glm::vec2 size = {glyph->x1() - glyph->x0(), glyph->y1() - glyph->y0()};
        glm::vec4 uvs = {glyph->x0(), glyph->y0(), glyph->x1(), glyph->y1()};
        uvs /= glm::vec4{texture->size, texture->size};
        batch.draw(*texture, pos + off, size, uvs);
        pos.x += glyph->xadvance();
    }
}