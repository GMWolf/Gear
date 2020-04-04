//
// Created by felix on 07/01/2020.
//

#include <gear/TextureAtlas.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <gear/Texture.h>
#include "gear/Texture.h"

gear::Sprite gear::TextureAtlas::getSprite(const std::string &name) const {
    return sprites.at(name);
}

gear::TextureAtlas gear::TextureAtlasLoader::load(const std::string &name, AssetRegistry& registry) {
    std::ifstream in(name);
    nlohmann::json j;
    in >> j;

    TextureAtlas atlas;

    auto texFile = j["texture"];
    registry.load<Texture>(texFile);
    atlas.texture = registry.get<Texture>(texFile);

    for(auto& o : j["sprites"]) {
        Sprite spr;
        spr.tex = atlas.texture;
        spr.size = {};

        auto& subimages = o["subimages"];

        spr.texRegions.reserve(subimages.size());
        for( auto& subimage : subimages) {
            int x = subimage["x"];
            int y = subimage["y"];
            int w = subimage["w"];
            int h = subimage["h"];

            TexRegion texRegion {};
            texRegion.uvs = glm::vec4{ x, y+h, x+w, y} / glm::vec4(atlas.texture->size, atlas.texture->size);
            texRegion.crop = {0,0,0,0};

            spr.texRegions.push_back(texRegion);
        }

        spr.size = glm::vec2{o["size"]["x"], o["size"]["y"]};
        spr.origin = glm::vec2(o["origin"]["x"], o["origin"]["y"]);
        spr.bbox.left = -spr.origin.x;
        spr.bbox.bottom = -spr.origin.y;
        spr.bbox.right = spr.size.x - spr.origin.x;
        spr.bbox.top = spr.size.y - spr.origin.y;

        atlas.sprites.emplace(std::make_pair(o["name"], spr));
    }

    return atlas;
}

