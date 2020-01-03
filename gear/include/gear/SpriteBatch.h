//
// Created by felix on 03/01/2020.
//

#ifndef GEAR_SPRITEBATCH_H
#define GEAR_SPRITEBATCH_H

#include <glad/glad.h>

namespace gear {

    class SpriteBatch {
        struct Vertex {
            float pos[2];
            float uv[2];
        };

    public:
        SpriteBatch() {
            glGenBuffers(1, &vbo);
            glGenVertexArrays(1, &vao);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        SpriteBatch(const SpriteBatch& spriteBatch) = delete;
        SpriteBatch& operator=(const SpriteBatch& spriteBatch) = delete;


        ~SpriteBatch() {
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
        }


        void draw(float x, float y, float w, float h) {

        }


        void flush() {

        }


    private:
        GLuint vbo{};
        GLuint vao{};
    };

}

#endif //GEAR_SPRITEBATCH_H
