//
// Created by felix on 05/01/2020.
//

#include <gear/SpriteBatch.h>
#include <gear/Texture.h>
#include <gear/Sprite.h>

void gear::SpriteBatch::draw(const Texture& tex, glm::vec2 pos, glm::vec2 size, glm::vec2 srcPos, glm::vec2 srcSize) {
    if (map == nullptr) {
        if (first < batchSize) {
            bufferUpdate();
        } else {
            bufferOrphan();
        }
    }

    if (batchTex == 0) {
        batchTex = tex.tex;
    } else if (batchTex != tex.tex) {
        flush();
        batchTex = tex.tex;
    }

    {
        auto *vertices = static_cast<Vertex *>(map) + count;
        vertices[0].pos = pos;
        vertices[0].uv  = srcPos;
        vertices[1].pos = pos + glm::vec2{0, size.y};
        vertices[1].uv = {srcPos.x, srcPos.y + srcSize.y};
        vertices[2].pos = pos + glm::vec2{size.x, 0};
        vertices[2].uv = {srcPos.x + srcSize.x, srcPos.y};
        vertices[3].pos = pos + glm::vec2{size.x, 0};
        vertices[3].uv = {srcPos.x + srcSize.x, srcPos.y};
        vertices[4].pos = pos + glm::vec2{0, size.y};
        vertices[4].uv = {srcPos.x, srcPos.y + srcSize.y};
        vertices[5].pos = pos + size;
        vertices[5].uv = srcPos + srcSize;
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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, batchTex);

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

void gear::SpriteBatch::draw(const gear::Sprite &sprite, glm::vec2 pos, glm::vec2 size) {
    auto tex = sprite.tex.lock();

    if (tex) {
        draw(*tex, pos, size, sprite.pos, sprite.size);
    }
}
