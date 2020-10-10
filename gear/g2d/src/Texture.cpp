//
// Created by felix on 05/01/2020.
//

#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>
#include <gear/sprite_generated.h>
#include <gear/texture_generated.h>
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

gear::Texture::Texture(GLuint tex, glm::ivec2 size) : tex(tex), size(size) {
}


gear::Texture gear::createTexture(const gear::assets::Texture* texDef, const char* name) {
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
        case gear::assets::PixelFormat::R8:
            internalFormat = GL_R8;
            format = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;
        case gear::assets::PixelFormat::RGB8:
            internalFormat = GL_RGB8;
            format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        case gear::assets::PixelFormat::RGBA8:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texDef->width(), texDef->height(), 0, format, type, buffer.data());

    glBindTexture(GL_TEXTURE_2D, 0);

    return gear::Texture(tex, {texDef->width(), texDef->height()});
}


gear::Texture *gear::TextureStore::getTexture(const gear::assets::Texture * tex) {
    auto it = textures.find(tex);
    if(it == textures.end()) {
        it = textures.insert({tex, createTexture(tex, nullptr)}).first;
    }
    return &it->second;
}

gear::Texture *gear::TextureStore::getTexture(const gear::assets::Ref* ref) {
    return nullptr;
}
