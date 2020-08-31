//
// Created by felix on 29/04/2020.
//

#ifndef GEAR_PRIMDRAW_H
#define GEAR_PRIMDRAW_H

#include <glad/glad.h>
#include <glm/vec2.hpp>

namespace gear {

    class PrimDraw {

        struct Vertex {
            glm::vec2 pos;
        };

    public:

        PrimDraw();

        PrimDraw(const PrimDraw &) = delete;

        PrimDraw(PrimDraw &&) = delete;

        PrimDraw &operator=(const PrimDraw &) = delete;

        PrimDraw &operator=(PrimDraw &&) = delete;

        ~PrimDraw();

        void flush();

        Vertex *map(size_t count);

        void line(glm::vec2 a, glm::vec2 b);

        void rect(glm::vec2 a, glm::vec2 b);

        void circle(glm::vec2 a, float r, int segments = 24);

    private:
        const size_t bufferSize = 1024;
        size_t index = 0;

        GLint *firsts;
        GLint *pFirst;
        GLint *counts;
        GLint *pCount;
        size_t primCount = 0;

        Vertex *ptr{};
        GLuint vbo{};
        GLuint vao{};
    };

}
#endif //GEAR_PRIMDRAW_H
