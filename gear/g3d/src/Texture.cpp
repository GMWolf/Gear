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

static bool formatIsCompressed(GLenum format) {
    switch (format) {
        case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
            return true;
        default:
            return false;
    }
}
void gear::g3d::Texture::subimage(GLint level_, GLint xoffset_, GLint yoffset_, GLsizei width_, GLsizei height_,
                                  GLenum format_, GLenum type_, size_t dataSize, const void *data) {
    if (formatIsCompressed(format_)) {
        glCompressedTextureSubImage2D(id, level_, xoffset_, yoffset_, width_, height_, format_, dataSize, data);
    } else {
        glTextureSubImage2D(id, level_, xoffset_, yoffset_, width_, height_, format_, type_, data);
    }
}
gear::g3d::Texture::~Texture() = default;
gear::g3d::Texture::Texture() = default;

gear::g3d::Texture gear::g3d::createTextureFromAsset(const gear::assets::Texture *texDef) {

    auto format = 0;
    auto type = 0;
    switch(texDef->format()) {
        case assets::PixelFormat::Invalid:
            break;
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
        case assets::PixelFormat::BC7:
            format = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
            type = GL_UNSIGNED_BYTE;
            break;
    }

    Texture texture;
    texture.create();
    texture.storage(texDef->width(), texDef->height(), texDef->mips()->size(), format);

    std::vector<char> buffer;

    for(size_t level = 0; level < texDef->mips()->size(); level++) {
        auto mipData = texDef->mips()->Get(level)->data();
        buffer.resize(ZSTD_getFrameContentSize(mipData->data(), mipData->size()));
        ZSTD_decompress(buffer.data(), buffer.size(), mipData->data(), mipData->size());

        texture.subimage(level, 0, 0, texDef->width() >> level, texDef->height() >> level, format, type, buffer.size(), buffer.data());
    }

    return texture;
}

const gear::g3d::Texture &gear::g3d::TextureCache::get(const gear::assets::Texture *tex) {
    auto it = textures.find(tex);
    if(it == textures.end()) {
        it = textures.insert({tex, createTextureFromAsset(tex)}).first;
    }
    return it->second;
}
