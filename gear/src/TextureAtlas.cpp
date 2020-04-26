//
// Created by felix on 07/01/2020.
//

#include <gear/TextureAtlas.h>
#include <fstream>
#include <ios>
#include <gear/Texture.h>
#include <generated/texture_atlas_generated.h>

gear::Sprite gear::TextureAtlas::getSprite(const std::string &name) const {
    return sprites.at(name);
}

gear::TextureAtlas gear::TextureAtlasLoader::load(const std::string &name, AssetRegistry& registry) {


    std::ifstream in(name, std::ios::binary);
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    in.seekg(0, std::ios::beg);
    auto buffer = (char*)malloc(size);
    in.read(buffer, size);

    auto atlasBin = gear::bin::GetAtlas(buffer);

    TextureAtlas atlas;
    registry.load<Texture>(atlasBin->texture()->str());
    atlas.texture = registry.get<Texture>(atlasBin->texture()->str());

    for(auto o : *atlasBin->sprites()) {
        Sprite spr;
        spr.tex = atlas.texture;
        spr.size = {};
        spr.texRegions.reserve(o->images()->size());
        for (auto i : *o->images()) {
            TexRegion region{};
            region.crop = {0, 0, 0, 0};
            region.uvs = glm::vec4{i->pos().x(), i->pos().y() + i->size().y(),
                                   i->pos().x() + i->size().x(), i->pos().y()};
            region.uvs /= glm::vec4(atlas.texture->size, atlas.texture->size);
            spr.texRegions.push_back(region);
        }

        spr.size = {o->size()->x(), o->size()->y()};
        spr.origin = {o->origin()->x(), o->origin()->y()};
        spr.bbox.left = -spr.origin.x;
        spr.bbox.bottom = -spr.origin.y;
        spr.bbox.right = spr.size.x - spr.origin.x;
        spr.bbox.top = spr.size.y - spr.origin.y;

        atlas.sprites.emplace(std::make_pair(o->name()->str(), spr));
    }

    free(buffer);

    return atlas;
}

