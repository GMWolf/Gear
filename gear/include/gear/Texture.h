//
// Created by felix on 05/01/2020.
//

#ifndef GEAR_TEXTURE_H
#define GEAR_TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <vector>
#include "AssetManager.h"

namespace gear {

    class Texture {
    public:
        explicit Texture(glm::vec4 color);
        Texture(GLuint tex, glm::ivec2 size);

        Texture(const Texture&) = delete;
        Texture&operator=(const Texture&) = delete;

        Texture(Texture&&) noexcept;
        Texture&operator=(Texture&&) noexcept ;

        ~Texture();
        GLuint tex {0};
        glm::ivec2 size {0,0};
    };

    struct TexRegion {
        glm::vec4 uvs; // min uv, max uv
        glm::vec4 crop; //left, top, right, bottom
    };

    //TODO split sprite into sprite component and sprite resource
    struct Sprite {
        glm::vec2 size {};
        glm::vec2 origin {};
        std::vector<TexRegion> texRegions {}; //TODO get rid of this allocation
        AssetReference<Texture> tex;
        uint16_t imageIndex {0};

        struct {
            float left;
            float bottom;
            float right;
            float top;
        } bbox;
    };

    class TextureLoader : public AssetLoader<Texture> {
    public:
        Texture load(const std::string& name, AssetRegistry& registry) override;
    };
}


#endif //GEAR_TEXTURE_H
