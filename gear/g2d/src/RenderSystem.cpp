//
// Created by felix on 01/04/2020.
//

#include <RenderSystem.h>
#include <gear/ecs/ECS.h>
#include <gear/View.h>
#include <gear/Transform.h>
#include <glm/gtc/type_ptr.hpp>
#include <SpriteBatch.h>
#include "PrimDraw.h"
#include <Shader.h>
#include <gear/CollisionShape.h>
#include "Texture.h"
#include <gear/sprite_generated.h>
#include "G2DInstance.h"


void gear::renderSprites(gear::G2DInstance* g2d, gear::ecs::Registry &ecs, const gear::assets::Shader* shader) {

    using namespace gear;
    static const ecs::Query viewQuery = ecs::Query().all<View>();
    static const ecs::Query spriteQuery = ecs::Query().all<Sprite, Transform>();

    const size_t spriteChunkArraySize = 1024;
    ecs::Chunk* spriteChunkArray[spriteChunkArraySize];
    auto spriteChunks = ecs.queryChunks(spriteQuery, spriteChunkArray, spriteChunkArraySize);

    const size_t viewChunkArraySize = 1;
    ecs::Chunk* viewChunkArray[viewChunkArraySize];
    auto viewChunks = ecs.queryChunks(viewQuery, viewChunkArray, viewChunkArraySize);

    auto shd = g2d->shaderStore->getShader(shader);

    shd->use();
    glUniform1i(shd->uniformLocation("tex"), 0);

    for(auto c : viewChunks) {
        for(auto [view] : ecs::ChunkView<View>(*c)) {

            auto vm = view.matrix();
            glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

            for(auto c : spriteChunks) {
                for(auto [sprite, transform] : ecs::ChunkView<Sprite, Transform>(*c)) {
                    
                    auto tex = g2d->textureStore->getTexture(sprite.tex);
                    gear::spriteBatchDraw(*g2d->spriteBatch, *tex, sprite.texRegions[sprite.imageIndex], transform.pos - sprite.origin, sprite.size);
                }
            }

        }
    }

    spriteBatchFlush(*g2d->spriteBatch);
}


void gear::renderDebugShapes(gear::G2DInstance* g2d, gear::ecs::Registry &ecs, const gear::assets::Shader* shader) {

    auto primDraw = g2d->primDraw;

    const size_t chunkArraySize = 1024;
    ecs::Chunk* chunkArray[chunkArraySize];
    {

        gear::View view {{0,0}, {480, 720}};

        auto shd = g2d->shaderStore->getShader(shader);

        shd->use();
        glUniform1i(shd->uniformLocation("tex"), 0);
        auto vm = view.matrix();
        glUniformMatrix4fv(shd->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

        auto chunks = ecs.queryChunks(ecs::Query().all<gear::Transform, gear::CollisionShape>(), chunkArray,
                                      chunkArraySize);

        for(auto c : chunks) {
            auto chunk = ecs::ChunkView<gear::Transform, gear::CollisionShape>(*c);
            for(auto[transform, shape] : chunk) {
                if (auto circle = std::get_if<gear::Circle>(&shape)) {
                    primDraw->circle(transform.pos + circle->center, circle->radius);
                }
                if (auto rect = std::get_if<gear::Rectangle>(&shape)) {
                    primDraw->rect(transform.pos + rect->min, transform.pos + rect->max);
                }
            }
        }
    }

    primDraw->flush();
}

