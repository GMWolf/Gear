//
// Created by felix on 31/10/2020.
//

#include "Texture.h"
#include <gear/texture_generated.h>
#include <zstd.h>

void gear::g3d::Texture::release() {
    glDeleteTextures(1, &id);
    id = 0;
}

void gear::g3d::Texture::create() {
    glCreateTextures(GL_TEXTURE_2D, 1, &id);
}

void gear::g3d::Texture::storage(GLsizei width_, GLsizei height_, GLsizei levels_, GLenum internalFormat_) {
    width = width_;
    height = height_;
    levels = levels_;
    format = internalFormat_;
    glTextureStorage2D(id, levels, format, width, height);
}

void gear::g3d::Texture::subimage(GLint level_, GLint xoffset_, GLint yoffset_, GLsizei width_, GLsizei height_,
                                  GLenum format_, GLenum type_, const void *data) {
    glTextureSubImage2D(id, level_, xoffset_, yoffset_, width_, height_, format_, type_, data);
}

gear::g3d::Texture::Texture() = default;

gear::g3d::Texture gear::g3d::createTextureFromAsset(gear::assets::Texture *texDef) {
    Texture texture;
    texture.create();
    texture.storage(texDef->width(), texDef->height(), 1, GL_SRGB8);

    std::vector<char> buffer(ZSTD_getFrameContentSize(texDef->data()->data(), texDef->data()->size()));
    ZSTD_decompress(buffer.data(), buffer.size(), texDef->data()->data(), texDef->data()->size());

    auto format = 0;
    auto type = 0;
    switch(texDef->format()) {
        case gear::assets::PixelFormat::R8:
            format = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;
        case gear::assets::PixelFormat::RGB8:
            format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        case gear::assets::PixelFormat::RGBA8:
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
    }

    texture.subimage(0, 0, 0, texDef->width(), texDef->height(), format, type, buffer.data());

    return texture;
}
