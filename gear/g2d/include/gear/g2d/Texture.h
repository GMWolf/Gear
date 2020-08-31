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

    struct Sprite {
        glm::vec2 size {};
        glm::vec2 origin {};
        std::vector<TexRegion> texRegions {}; //TODO get rid of this allocation
        const Texture* tex;
        uint16_t imageIndex {0};

        std::optional<CollisionShape> mask;
    };

    Sprite createSprite(const assets::Sprite* spriteDef, TextureStore& textureStore);
}


#endif //GEAR_TEXTURE_H
