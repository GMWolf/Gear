//
// Created by felix on 05/01/2020.
//

#include <gear/SpriteBatch.h>
#include <gear/Texture.h>

void gear::SpriteBatch::draw(const Texture& tex, float x, float y, float w, float h) {
    if (map == nullptr) {
        if (first < batchSize) {
            bufferUpdate();
        } else {
            bufferOrphan();
        }
    }


    {
        auto *vertices = static_cast<Vertex *>(map) + count;
        vertices[0].pos[0] = x;
        vertices[0].pos[1] = y;
        vertices[1].pos[0] = x;
        vertices[1].pos[1] = y + h;
        vertices[2].pos[0] = x + w;
        vertices[2].pos[1] = y;
        vertices[3].pos[0] = x + w;
        vertices[3].pos[1] = y;
        vertices[4].pos[0] = x;
        vertices[4].pos[1] = y + h;
        vertices[5].pos[0] = x + w;
        vertices[5].pos[1] = y + h;
    }

    count += 6;

    if (first + count >= batchSize) {
        flush();
    }
}

void gear::SpriteBatch::flush() {

    if (count > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        //Flush range
        glFlushMappedBufferRange(GL_ARRAY_BUFFER, first * sizeof(Vertex), count * sizeof(Vertex));
        glUnmapBuffer(GL_ARRAY_BUFFER);
        map = nullptr;

        //Issue draw call
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, first, count);
        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        first += count;
        count = 0;
    }
}

void gear::SpriteBatch::bufferUpdate() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    map = glMapBufferRange(GL_ARRAY_BUFFER, first * sizeof(Vertex), (batchSize * 6 - first) * sizeof(Vertex),
                           GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_RANGE_BIT |
                           GL_MAP_FLUSH_EXPLICIT_BIT);
}

void gear::SpriteBatch::bufferOrphan() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    map = glMapBufferRange(GL_ARRAY_BUFFER, 0, batchSize * 6 * sizeof(Vertex),
                           GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    first = 0;
    count = 0;
}

gear::SpriteBatch::~SpriteBatch() {
    if (map) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

gear::SpriteBatch::SpriteBatch(size_t size) : batchSize(size) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size * 6 * sizeof(Vertex), nullptr, GL_STREAM_DRAW);

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
