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
#include <gear/Assets.h>
#include <gear/CollisionShape.h>
#include <unordered_map>
#include "SpriteBatch.h"

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

    namespace assets {
        class Sprite;
        class Texture;
    }

    class TextureStore {
        std::unordered_map<const assets::Texture*, Texture> textures;
    public:
        Texture* getTexture(const assets::Texture*);
    };

    Texture createTexture(const assets::Texture* texDef, const char* name = nullptr);
}


#endif //GEAR_TEXTURE_H
