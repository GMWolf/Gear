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
            region.uvs /= glm::vec4{spr.tex->size, spr.tex->size};
            spr.texRegions.push_back(region);
        }

        spr.size = {o->size()->x(), o->size()->y()};

        //collision shape

        bool hasOrigin = false;
        if (auto origin = o->objects()->LookupByKey("origin")) {
            spr.origin.x = origin->shape_as_point()->x();
            spr.origin.y = origin->shape_as_point()->y();
        } else {
            if (auto col = o->objects()->LookupByKey("collision")) {
                if (auto circle = col->shape_as_circle()) {
                    spr.origin.x = circle->x() + circle->r();
                    spr.origin.y = circle->y() + circle->r();
                } else if (auto rect = col->shape_as_rectangle()) {
                    spr.origin.x = rect->x() + rect->w() / 2;
                    spr.origin.y = rect->y() + rect->h() / 2;
                } else if (auto point = col->shape_as_point()){
                    spr.origin.x = point->x();
                    spr.origin.y = point->y();
                } else {
                    spr.origin.x = 0;
                    spr.origin.y = 0;
                }

                //make origin relative to bottom left
                spr.origin.y = spr.size.y - spr.origin.y;

            } else {
                spr.origin.x = 0;
                spr.origin.y = 0;
            }
        }


        if (auto col = o->objects()->LookupByKey("collision")) {
            if (auto rect = col->shape_as_rectangle()) {
                spr.mask = Rectangle {
                    {rect->x() - spr.origin.x, rect->y() - spr.origin.y},
                    {rect->x() + rect->w() - spr.origin.x, rect->y() + rect->h() - spr.origin.y}
                };
            } else if (auto circle = col->shape_as_circle()) {
                float x = circle->x();
                float y = spr.size.y - circle->y() - circle->r() * 2;
                spr.mask = Circle {
                        {(x + circle->r()) - spr.origin.x, (y + circle->r()) - spr.origin.y},
                        circle->r()
                };
            }
        }


        atlas.sprites.emplace(std::make_pair(o->name()->str(), spr));
    }

    free(buffer);

    return atlas;
}

