//
// Created by felix on 07/01/2020.
//

#include <gear/TextureAtlas.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "gear/Texture.h"


gear::TextureAtlas::TextureAtlas(const std::string &name) {
    std::ifstream in(name);
    nlohmann::json j;
    in >> j;

    auto texFile = j["texture"];
    texture = std::make_shared<Texture>(texFile);


    for(auto& o : j["sprites"]) {
        Sprite spr;

        int x = o["x"];
        int y = o["y"];
        int w = o["w"];
        int h = o["h"];

        spr.uv = glm::vec4{x, y + h, x + w, y} / glm::vec4{texture->size, texture->size};
        spr.size = glm::vec2{w, h};
        spr.tex = texture;
        sprites.emplace(std::make_pair(o["name"], spr));
    }
}

gear::Sprite gear::TextureAtlas::getSprite(const std::string &name) const {
    return sprites.at(name);
}
