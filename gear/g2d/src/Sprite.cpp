//
// Created by felix on 01/09/2020.
//

#include <Sprite.h>


gear::Sprite gear::createSprite(const gear::assets::Sprite* spriteDef) {
    gear::Sprite sprite;
    sprite.tex = (assets::Texture*)spriteDef->texture()->ptr();
    sprite.size = {};
    sprite.texRegions.reserve(spriteDef->images()->size());
    for (auto uvs : *spriteDef->images()) {
        gear::TexRegion region{};
        region.crop = {0, 0, 0, 0};
        region.uvs = glm::vec4{uvs->x0(), uvs->y0(), uvs->x1(), uvs->y1()};
        glm::vec2 size = {sprite.tex->width(), sprite.tex->height()};
        region.uvs /= glm::vec4{size, size};
        sprite.texRegions.push_back(region);
    }

    sprite.size = {spriteDef->size()->x(), spriteDef->size()->y()};

    //collision shape

    bool hasOrigin = false;
    if (auto origin = spriteDef->objects()->LookupByKey("origin")) {
        sprite.origin.x = origin->shape_as_point()->x();
        sprite.origin.y = origin->shape_as_point()->y();
    } else {
        if (auto col = spriteDef->objects()->LookupByKey("collision")) {
            if (auto circle = col->shape_as_circle()) {
                sprite.origin.x = circle->x() + circle->r();
                sprite.origin.y = circle->y() + circle->r();
            } else if (auto rect = col->shape_as_rectangle()) {
                sprite.origin.x = rect->x() + rect->w() / 2;
                sprite.origin.y = rect->y() + rect->h() / 2;
            } else if (auto point = col->shape_as_point()){
                sprite.origin.x = point->x();
                sprite.origin.y = point->y();
            } else {
                sprite.origin.x = 0;
                sprite.origin.y = 0;
            }

            //make origin relative to bottom left
            sprite.origin.y = sprite.size.y - sprite.origin.y;

        } else {
            sprite.origin.x = 0;
            sprite.origin.y = 0;
        }
    }


    if (auto col = spriteDef->objects()->LookupByKey("collision")) {
        if (auto rect = col->shape_as_rectangle()) {
            sprite.mask = gear::Rectangle {
                    {rect->x() - sprite.origin.x, rect->y() - sprite.origin.y},
                    {rect->x() + rect->w() - sprite.origin.x, rect->y() + rect->h() - sprite.origin.y}
            };
        } else if (auto circle = col->shape_as_circle()) {
            float x = circle->x();
            float y = sprite.size.y - circle->y() - circle->r() * 2;
            sprite.mask = gear::Circle {
                    {(x + circle->r()) - sprite.origin.x, (y + circle->r()) - sprite.origin.y},
                    circle->r()
            };
        }
    }

    return sprite;
}