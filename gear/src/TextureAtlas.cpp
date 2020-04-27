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
        for (auto uvs : *o->images()) {
            TexRegion region{};
            region.crop = {0, 0, 0, 0};
            region.uvs = glm::vec4{uvs->x0(), uvs->y0(), uvs->x1(), uvs->y1()};
            spr.texRegions.push_back(region);
        }

        spr.size = {o->size()->x(), o->size()->y()};

        //collision shape

        bool hasOrigin = false;
        if (auto origin = o->objects()->LookupByKey("origin")) {
            spr.origin.x = origin->x();
            spr.origin.y = origin->y();
        } else {
            if (auto col = o->objects()->LookupByKey("collision")) {
                if (col->shape() == gear::bin::Shape_Circle) {
                    spr.origin.x = col->x() + col->w() / 2;
                    spr.origin.y = col->y() + col->w() / 2;
                } else if (col->shape() == gear::bin::Shape_Rectangle) {
                    spr.origin.x = col->x() + col->w() / 2;
                    spr.origin.y = col->y() + col->h() / 2;
                } else {
                    spr.origin.x = col->x();
                    spr.origin.y = col->y();
                }

                //make origin relative to bottom left
                spr.origin.y = spr.size.y - spr.origin.y;

            } else {
                spr.origin.x = 0;
                spr.origin.y = 0;
            }
        }


        if (auto col = o->objects()->LookupByKey("collision")) {
            if (col->shape() == gear::bin::Shape_Rectangle) {
                spr.mask = Rectangle {
                    {col->x() - spr.origin.x, col->y() - spr.origin.y},
                    {col->x() + col->w() - spr.origin.x, col->y() + col->h() - spr.origin.y}
                };
            } else if (col->shape() == gear::bin::Shape_Circle) {
                float x = col->x();
                float y = spr.size.y - col->y() - col->w();
                spr.mask = Circle {
                        {(x + col->w() / 2) - spr.origin.x, (y + col->w() / 2) - spr.origin.y},
                        col->w() / 2
                };
            }
        }


        atlas.sprites.emplace(std::make_pair(o->name()->str(), spr));
    }

    free(buffer);

    return atlas;
}

