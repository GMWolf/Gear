//
// Created by felix on 05/01/2020.
//

#include <SpriteBatch.h>
#include <Texture.h>

#include <glm/vec2.hpp>

struct Vertex {
    glm::vec2 pos;
    glm::vec2 uv;
};

void gear::SpriteBatch::flush() {

    if (count > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        //Flush range
        glFlushMappedBufferRange(GL_ARRAY_BUFFER, first * 4 * sizeof(Vertex), count * 4 * sizeof(Vertex));
        glUnmapBuffer(GL_ARRAY_BUFFER);
        map = nullptr;

        //Set tex
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, batchTex);

        //Issue draw call
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_SHORT, (void*)(first * 6 * sizeof(GLushort)));
        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        first += count;
        count = 0;
    }
}

void gear::SpriteBatch::bufferUpdate() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    map = glMapBufferRange(GL_ARRAY_BUFFER, 0, batchSize * 4 * sizeof(Vertex),
                           GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_RANGE_BIT |
                           GL_MAP_FLUSH_EXPLICIT_BIT);
}

void gear::SpriteBatch::bufferOrphan() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    map = glMapBufferRange(GL_ARRAY_BUFFER, 0, batchSize * 4 * sizeof(Vertex),
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
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(Vertex), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * 6 * sizeof(GLushort), nullptr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    fillElementBuffer();

    nulltex = std::make_unique<Texture>(glm::vec4{1, 0, 1, 1});
}


void gear::SpriteBatch::draw(const Texture& tex, glm::vec2 pos, glm::vec2 size, glm::vec4 uv) {
    if (batchTex == 0) {
        batchTex = tex.tex;
    } else if (batchTex != tex.tex) {
        flush();
        batchTex = tex.tex;
    }

    if (map == nullptr) {
        if (first < batchSize) {
            bufferUpdate();
        } else {
            bufferOrphan();
        }
    }

    {
        auto *vertices = static_cast<Vertex *>(map) + ((first + count) * 4);
        vertices[0].pos = pos;
        vertices[0].uv  = {uv.x, uv.y};
        vertices[1].pos = pos + glm::vec2{0, size.y};
        vertices[1].uv = {uv.x, uv.w};
        vertices[2].pos = pos + glm::vec2{size.x, 0};
        vertices[2].uv = {uv.z, uv.y};
        vertices[3].pos = pos + size;
        vertices[3].uv = {uv.z, uv.w};
    }

    count += 1;

    if (first + count >= batchSize) {
        flush();
    }
}

void gear::SpriteBatch::draw(const Texture& tex, const TexRegion& texRegion, glm::vec2 pos, glm::vec2 size) {
    draw(tex, pos, size, texRegion.uvs);
}

void gear::SpriteBatch::fillElementBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    auto* elements = static_cast<GLushort*>(glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, batchSize * 6 * sizeof(GLushort), GL_MAP_WRITE_BIT));
    for(int i = 0; i < batchSize; i++) {
        elements[i * 6] = i * 4;
        elements[i * 6 + 1] = i * 4 + 1;
        elements[i * 6 + 2] = i * 4 + 2;
        elements[i * 6 + 3] = i * 4 + 1;
        elements[i * 6 + 4] = i * 4 + 2;
        elements[i * 6 + 5] = i * 4 + 3;
    }

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}
