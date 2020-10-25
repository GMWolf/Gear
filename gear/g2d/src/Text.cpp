//
// Created by felix on 24/02/2020.
//

#include <Text.h>
#include "SpriteBatch.h"
#include <gear/bitmapFont_generated.h>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include <g2d.h>
#include "FontCache.h"
#include "Shader.h"


void gear::renderText(gear::G2DInstance& g2d, const std::string &text, const gear::assets::BitmapFont* font, glm::vec2 pos, const gear::assets::Shader* shader, gear::View view) {

    auto shd = g2d.shaderStore->getShader(shader);
    shd->use();
    glUniform1i(shd->uniformLocation("tex"), 0);
    auto vm = view.matrix();
    glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

    auto texture = g2d.textureStore->getTexture((gear::assets::Texture*)font->texture()->ptr());

    for(char c: text) {
        auto glyph = font->glyphs()->Get(c - font->range_start());
        glm::vec2 off = {glyph->xoff(), glyph->yoff2()};
        glm::vec2 size = {glyph->x1() - glyph->x0(), glyph->y1() - glyph->y0()};
        glm::vec4 uvs = {glyph->x0(), glyph->y0(), glyph->x1(), glyph->y1()};
        uvs /= glm::vec4{texture->size, texture->size};
        g2d.spriteBatch->draw(*texture, pos + off, size, uvs);
        pos.x += glyph->xadvance();
    }

    g2d.spriteBatch->flush();
}

void gear::testTex(gear::G2DInstance& g2d) {
    g2d.spriteBatch->draw(*g2d.fontCache->texture, {0,0}, {1024, 1024}, {0, 0, 1, 1});
}

void gear::fontCacheAddChar(gear::G2DInstance &g2d, const gear::assets::Font *font, char c) {
    g2d.fontCache->addGlyph(GlyphEntry{
       .font = font,
       .glyphIndex = (uint32_t)c,
       .pixelWidth = 0,
       .pixelHeight = 164,
    });
}
