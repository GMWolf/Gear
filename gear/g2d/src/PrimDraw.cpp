//
// Created by felix on 29/04/2020.
//

#include "PrimDraw.h"
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

gear::PrimDraw::PrimDraw() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
    ptr = (Vertex*)glMapBufferRange(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(Vertex),
                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    firsts = new GLint[bufferSize];
    pFirst = firsts;
    counts = new GLint[bufferSize];
    pCount = counts;
}

gear::PrimDraw::Vertex *gear::PrimDraw::map(size_t count) {
    if (index + count > bufferSize) {
        flush();
    }
    auto ret = ptr + index;
    *pFirst = index;
    pFirst++;
    *pCount = count;
    pCount++;
    primCount++;

    index += count;
    return ret;
}

void gear::PrimDraw::flush() {
    if (index > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, index * sizeof(Vertex));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        glBindVertexArray(vao);
        glMultiDrawArrays(GL_LINE_STRIP, firsts, counts, primCount);
        glBindVertexArray(0);

        ptr = (Vertex*)glMapBufferRange(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(Vertex),
                GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        pFirst = firsts;
        pCount = counts;
        primCount = 0;
        index = 0;
    }
}

gear::PrimDraw::~PrimDraw() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete[] firsts;
    delete[] counts;
}

void gear::PrimDraw::line(glm::vec2 a, glm::vec2 b) {
    Vertex* v = map(2);
    v[0].pos = a;
    v[1].pos = b;
}

void gear::PrimDraw::rect(glm::vec2 a, glm::vec2 b) {
    Vertex* v = map(5);
    v[0].pos = a;
    v[1].pos = {b.x, a.y};
    v[2].pos = b;
    v[3].pos = {a.x, b.y};
    v[4].pos = a;
}

void gear::PrimDraw::circle(glm::vec2 a, float r, int segments) {
    Vertex* v = map(segments + 1);
    for(int i = 0; i < segments; i++) {
        float alpha = (float) i * glm::two_pi<float>() / (float) segments;
        v[i].pos = a + glm::vec2{glm::sin(alpha), glm::cos(alpha)} * r;
    }
    v[segments] = v[0];
}
