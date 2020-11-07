//
// Created by felix on 31/10/2020.
//

#ifndef GEAR_TEXTURE_H
#define GEAR_TEXTURE_H

#include <glad/glad.h>
#include "GlObject.h"
#include <unordered_map>

namespace gear::assets {
    struct Texture;
}

namespace gear::g3d {

    struct Texture : public GlObject<Texture> {
        Texture();
        Texture(Texture&&) = default;
        Texture& operator=(Texture&&) = default;
        ~Texture();
        void release();
        void create();
        void storage(GLsizei width_, GLsizei height_, GLsizei levels_, GLenum internalFormat_);
        void subimage(GLint level_, GLint xoffset_, GLint yoffset_, GLsizei width_, GLsizei height_, GLenum format_, GLenum type_, const void* data);

        GLsizei width{};
        GLsizei height{};
        GLsizei levels{};
        GLsizei format{};
    };

    struct TextureCache {
        std::unordered_map<const gear::assets::Texture*, Texture> textures;

        const Texture& get(const assets::Texture* tex);
    };

    Texture createTextureFromAsset(const assets::Texture* texDesc);
}
#endif //GEAR_TEXTURE_H
