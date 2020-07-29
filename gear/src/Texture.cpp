//
// Created by felix on 05/01/2020.
//

#include <gear/Texture.h>
#include <glm/gtc/type_ptr.hpp>
#include <generated/sprite_generated.h>
#include <generated/texture_generated.h>
#include <zstd.h>

gear::Texture::Texture(gear::Texture && o) noexcept : size(o.size), tex(o.tex) {
    o.size = {0,0};
    o.tex = 0;
}

gear::Texture &gear::Texture::operator=(Texture&& o) noexcept {
    glDeleteTextures(1, &tex);
    tex = o.tex;
    size = o.size;

    o.tex = 0;
    o.size = {0,0};

    return *this;
}


gear::Texture::~Texture() {
    glDeleteTextures(1, &tex);
}

gear::Texture::Texture(glm::vec4 color) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_FLOAT, glm::value_ptr(color));

    glBindTexture(GL_TEXTURE_2D, 0);

    size = {1,1};
}

gear::Texture::Texture(GLuint tex, glm::ivec2 size) : tex(tex), size(size){
}


gear::Texture gear::TextureLoader::load(const gear::assets::Texture* texDef, gear::AssetRegistry &registry, const char* name) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    if (name)
        glObjectLabel(GL_TEXTURE, tex, -1, name);

    std::vector<char> buffer(ZSTD_getFrameContentSize(texDef->data()->data(), texDef->data()->size()));
    ZSTD_decompress(buffer.data(), buffer.size(), texDef->data()->data(), texDef->data()->size());

    auto internalFormat = 0;
    auto format = 0;
    auto type = 0;
    switch(texDef->format()) {
        case assets::PixelFormat_R8:
            internalFormat = GL_R8;
            format = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;
        case assets::PixelFormat_RGB8:
            internalFormat = GL_RGB8;
            format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        case assets::PixelFormat_RGBA8:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texDef->width(), texDef->height(), 0, format, type, buffer.data());

    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture(tex, {texDef->width(), texDef->height()});
}

gear::Sprite gear::SpriteLoader::load(const gear::assets::Sprite* spriteDef, gear::AssetRegistry &registry) {
    Sprite sprite;
    sprite.tex = registry.getTexture(spriteDef->texture()->str());
    sprite.size = {};
    sprite.texRegions.reserve(spriteDef->images()->size());
    for (auto uvs : *spriteDef->images()) {
        TexRegion region{};
        region.crop = {0, 0, 0, 0};
        region.uvs = glm::vec4{uvs->x0(), uvs->y0(), uvs->x1(), uvs->y1()};
        region.uvs /= glm::vec4{sprite.tex->size, sprite.tex->size};
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
            sprite.mask = Rectangle {
                    {rect->x() - sprite.origin.x, rect->y() - sprite.origin.y},
                    {rect->x() + rect->w() - sprite.origin.x, rect->y() + rect->h() - sprite.origin.y}
            };
        } else if (auto circle = col->shape_as_circle()) {
            float x = circle->x();
            float y = sprite.size.y - circle->y() - circle->r() * 2;
            sprite.mask = Circle {
                    {(x + circle->r()) - sprite.origin.x, (y + circle->r()) - sprite.origin.y},
                    circle->r()
            };
        }
    }

    return sprite;
}
