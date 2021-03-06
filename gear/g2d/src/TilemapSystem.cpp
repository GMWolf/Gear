//
// Created by felix on 07/04/2020.
//

#include <TilemapSystem.h>
#include <gear/TileMap.h>
#include <gear/ecs/ECS.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <gear/Transform.h>
#include <gear/View.h>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <gear/texture_generated.h>
#include <gear/tileset_generated.h>
#include "Texture.h"

namespace gecs = gear::ecs;


struct TilemapVertex {
    glm::vec2 pos;
    glm::vec2 uv;
};

struct TilemapSystemComponent {
    size_t count;
    GLuint vertexArray;
    union {
        struct {
            GLuint vertexBuffer;
            GLuint elementBuffer;
        };
        GLuint buffers[2];
    };
};

void gear::tilemapSystemCreateSystemComponent(ecs::Registry &ecs, gecs::CommandBuffer &cmd) {

    gecs::Chunk *chunkArray[512];
    auto chunks = ecs.queryChunks(gecs::Query().all<TilemapComponent>().none<TilemapSystemComponent>(), chunkArray,
                                  512);


    for (auto c : chunks) {
        auto chunk = gecs::ChunkView<gecs::EntityRef, TilemapComponent>(*c);

        for (auto[entity, tilemap] : chunk) {
            size_t vertexCount = tilemap.width * tilemap.height * 4;
            size_t elementCount = tilemap.width * tilemap.height * 6;

            TilemapSystemComponent tsc{0, 0};
            //create VAO
            glGenVertexArrays(1, &tsc.vertexArray);
            glBindVertexArray(tsc.vertexArray);
            //create buffers
            glGenBuffers(2, tsc.buffers);
            glBindBuffer(GL_ARRAY_BUFFER, tsc.vertexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tsc.elementBuffer);
            glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(TilemapVertex), nullptr, GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(GLushort), nullptr, GL_STATIC_DRAW);
            auto *vertices = static_cast<TilemapVertex *>(glMapBufferRange(GL_ARRAY_BUFFER, 0,
                                                                           vertexCount * sizeof(TilemapVertex),
                                                                           GL_MAP_WRITE_BIT));
            auto *elements = static_cast<GLushort *>(glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
                                                                      elementCount * sizeof(GLushort),
                                                                      GL_MAP_WRITE_BIT));

            for (int y = 0; y < tilemap.height; y++) {
                for (int x = 0; x < tilemap.width; x++) {
                    size_t tileIndex = x + y * tilemap.width;
                    auto tile = tileDataDecode(tilemap.data[tileIndex]);
                    auto tileVertexOffset = (x + y * tilemap.width) * 4;
                    auto tileElementOffset = (x + y * tilemap.width) * 6;
                    auto uvs = getTileUVs(tilemap.tileSet, tile.id, tile.hflip, tile.vflip, tile.dflip);
                    //Write vertices
                    auto *tileVertices = vertices + tileVertexOffset;
                    tileVertices[0].pos = {x * tilemap.tileSet->tileWidth(), y * tilemap.tileSet->tileHeight()};
                    tileVertices[0].uv = {uvs.x, uvs.y};
                    tileVertices[1].pos = {x * tilemap.tileSet->tileWidth(), (y + 1) * tilemap.tileSet->tileHeight()};
                    tileVertices[1].uv = {uvs.x, uvs.w};
                    tileVertices[2].pos = {(x + 1) * tilemap.tileSet->tileWidth(), y * tilemap.tileSet->tileHeight()};
                    tileVertices[2].uv = {uvs.z, uvs.y};
                    tileVertices[3].pos = {(x + 1) * tilemap.tileSet->tileWidth(), (y + 1) * tilemap.tileSet->tileHeight()};
                    tileVertices[3].uv = {uvs.z, uvs.w};
                    //Write elements;
                    auto *tileElements = elements + tileElementOffset;
                    tileElements[0] = tileVertexOffset;
                    tileElements[1] = tileVertexOffset + 1;
                    tileElements[2] = tileVertexOffset + 2;
                    tileElements[3] = tileVertexOffset + 1;
                    tileElements[4] = tileVertexOffset + 2;
                    tileElements[5] = tileVertexOffset + 3;
                }
            }

            glUnmapBuffer(GL_ARRAY_BUFFER);
            glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

            //vao vertices
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TilemapVertex),
                                  (void *) offsetof(TilemapVertex, pos));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TilemapVertex),
                                  (void *) offsetof(TilemapVertex, uv));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);

            tsc.count = tilemap.width * tilemap.height * 6;

            cmd.createComponent(entity, tsc);
        }

    }
}

void gear::tilemapSystemRender(gear::G2DInstance& g2d, ecs::Registry &ecs, const gear::assets::Shader* shader) {

    gecs::Chunk *chunkArray[512];
    auto chunks = ecs.queryChunks(gecs::Query().all<Transform, TilemapComponent, TilemapSystemComponent>(), chunkArray,
                                  512);


    ecs::Chunk *viewChunkArray[1];
    auto viewChunks = ecs.queryChunks(ecs::Query().all<View>(), viewChunkArray, 1);

    auto shd = g2d.shaderStore->getShader(shader);

    shd->use();

    for (auto viewChunk : viewChunks) {
        for (auto[view] : gecs::ChunkView<View>(*viewChunk)) {
            for (auto chunk : chunks) {
                for (auto[transform, tc, tsc] : gecs::ChunkView<Transform, TilemapComponent, TilemapSystemComponent>(
                        *chunk)) {
                    auto tview = view;
                    tview.pos -= transform.pos;
                    auto vm = tview.matrix();

                    auto tex = g2d.textureStore->getTexture((assets::Texture*)tc.tileSet->texture()->ptr());

                    glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, tex->tex);
                    glBindVertexArray(tsc.vertexArray);
                    glDrawElements(GL_TRIANGLES, tsc.count, GL_UNSIGNED_SHORT, nullptr);
                }
            }
        }
    }
    glBindVertexArray(0);
}