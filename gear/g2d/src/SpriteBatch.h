//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_SPRITEBATCH_H
#define GEAR_SPRITEBATCH_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Sprite.h"
#include <glad/glad.h>

namespace gear {

    class Texture;

    class SpriteBatch {
    public:
        explicit SpriteBatch(const SpriteBatchCreateInfo& createInfo);

        SpriteBatch(const SpriteBatch& spriteBatch) = delete;
        SpriteBatch& operator=(const SpriteBatch& spriteBatch) = delete;


        ~SpriteBatch();

        void draw(const gear::Texture& tex, glm::vec2 pos, glm::vec2 size, glm::vec4 uv = {0, 0, 1, 1});

        void flush();

    private:
        const size_t batchSize; //number of sprites in a batch

        /// Map the remainder of the buffer range UNSYNCHRONIZED
        void bufferUpdate();

        /// Map the entire buffer orphaning the previous data
        void bufferOrphan();

        void fillElementBuffer();

        void* map = nullptr;
        size_t count = 0;
        size_t first = 0;

        GLuint batchTex = 0;

        union {
            struct {
                GLuint vbo{};
                GLuint ebo{};
            };
            GLuint buffers[2];
        };
        GLuint vao{};

        std::unique_ptr<gear::Texture> nulltex;
    };
}

#endif //GEAR_SPRITEBATCH_H
