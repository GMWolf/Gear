//
// Created by felix on 24/02/2020.
//

#include <Text.h>
#include <SpriteBatch.h>
#include <gear/font_generated.h>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "G2DInstance.h"


void gear::renderText(gear::G2DInstance* g2d, const std::string &text, const gear::assets::Font* font, glm::vec2 pos, const gear::assets::Shader* shader, gear::View view) {

    auto shd = g2d->shaderStore->getShader(shader);
    shd->use();
    glUniform1i(shd->uniformLocation("tex"), 0);
    auto vm = view.matrix();
    glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

    auto texture = g2d->textureStore->getTexture((gear::assets::Texture*)font->texture()->ptr());

    for(char c: text) {
        auto glyph = font->glyphs()->Get(c - font->range_start());
        glm::vec2 off = {glyph->xoff(), glyph->yoff2()};
        glm::vec2 size = {glyph->x1() - glyph->x0(), glyph->y1() - glyph->y0()};
        glm::vec4 uvs = {glyph->x0(), glyph->y0(), glyph->x1(), glyph->y1()};
        uvs /= glm::vec4{texture->size, texture->size};
        spriteBatchDraw(*g2d->spriteBatch, *texture, pos + off, size, uvs);
        pos.x += glyph->xadvance();
    }

    spriteBatchFlush(*g2d->spriteBatch);
}