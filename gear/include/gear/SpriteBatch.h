//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_SPRITEBATCH_H
#define GEAR_SPRITEBATCH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

namespace gear {

    class Texture;
    class Sprite;

    class SpriteBatch {

        struct Vertex {
            glm::vec2 pos;
            glm::vec2 uv;
        };

    public:
        explicit SpriteBatch(size_t size);

        SpriteBatch(const SpriteBatch& spriteBatch) = delete;
        SpriteBatch& operator=(const SpriteBatch& spriteBatch) = delete;


        ~SpriteBatch();

        void draw(const Texture& tex, glm::vec2 pos, glm::vec2 size, glm::vec4 uv = {0, 0, 1, 1});

        void draw(const Sprite& sprite, glm::vec2 pos, glm::vec2 size);

        void draw(const Sprite& sprite, glm::vec2 pos);

        void flush();

    private:
        const size_t batchSize; //number of sprites in a batch

        /// Map the remainder of the buffer range UNSYNCHRONIZED
        void bufferUpdate();

        /// Map the entire buffer orphaning the previous data
        void bufferOrphan();


        void* map = nullptr;
        size_t count = 0;
        size_t first = 0;

        GLuint batchTex = 0;

        GLuint vbo{};
        GLuint vao{};

        std::unique_ptr<Texture> nulltex;
    };

}

#endif //GEAR_SPRITEBATCH_H
