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
        spr.pos = glm::vec2{ o["x"], o["y"] } / glm::vec2{texture->size};
        spr.size = glm::vec2{ o["w"], o["h"]} / glm::vec2{texture->size};
        spr.tex = texture;
        sprites.emplace(std::make_pair(o["name"], spr));
    }
}

gear::Sprite gear::TextureAtlas::getSprite(const std::string &name) const {
    return sprites.at(name);
}
