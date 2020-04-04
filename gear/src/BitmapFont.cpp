//
// Created by felix on 24/02/2020.
//

#include <gear/BitmapFont.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <gear/SpriteBatch.h>

namespace nj = nlohmann;

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
    std::ifstream in(name);
    nj::json j;
    in >> j;

    BitmapFont font;

    auto texFile = j["texture"];
    registry.load<Texture>(texFile);
    font.texture = registry.get<Texture>(texFile);

    font.rangeStart = j["rangeStart"];
    font.rangeCount = j["rangeCount"];

    auto c = j["rangeCount"].size();
    assert(j["glyphs"].size() == font.rangeCount);

    font.glyphs.reserve(font.rangeCount);

    for(auto& g : j["glyphs"]) {

        BitmapFont::Glyph glyph {};
        glyph.uv.x = g["x0"];
        glyph.uv.y = g["y1"];
        glyph.uv.z = g["x1"];
        glyph.uv.w = g["y0"];
        glyph.size = {glyph.uv.z - glyph.uv.x, glyph.uv.y - glyph.uv.w};
        glyph.uv /= glm::vec4{font.texture->size, font.texture->size};
        glyph.advance = g["xadvance"];
        glyph.offset.x = g["xoff"];
        glyph.offset.y = g["yoff2"];

        font.glyphs.push_back(glyph);
    }

    return font;
}
