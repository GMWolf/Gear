//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_SPRITEBATCH_H
#define GEAR_SPRITEBATCH_H

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace gear {

    class Texture;
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

        void draw(const Texture& tex, glm::vec2 pos, glm::vec2 size);

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
    };

}

#endif //GEAR_SPRITEBATCH_H
