//
// Created by felix on 01/04/2020.
//

#include <RenderSystem.h>
#include <gear/ecs/ECS.h>
#include <gear/View.h>
#include <gear/Transform.h>
#include <glm/gtc/type_ptr.hpp>
#include "SpriteBatch.h"
#include "PrimDraw.h"
#include "Shader.h"
#include <gear/CollisionShape.h>
#include <gear/g2d/Text.h>
#include "Texture.h"


template<class Fun>
static void foreachView(gear::ecs::Registry &ecs, Fun fun) {
    using namespace gear;
    static const ecs::Query viewQuery = gear::ecs::Query().all<View>();

    for(auto c : ecs.query(viewQuery)) {
        for(auto[view] : ecs::ChunkView<View>(*c)) {
            fun(view);
        }
    }
}


void gear::renderSprites(gear::G2DInstance* g2d, gear::ecs::Registry &ecs, const gear::assets::Shader* shader, gear::ecs::CommandBuffer& cmd) {

    using namespace gear;

    static const ecs::Query viewQuery = gear::ecs::Query().all<View>();
    static const ecs::Query spriteQuery = ecs::Query().all<SpriteComponent, Transform>();

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
                for(auto [sprite, transform] : ecs::ChunkView<SpriteComponent, Transform>(*c)) {
                    auto tex = g2d->textureStore->getTexture((gear::assets::Texture*)sprite.sprite->texture()->ptr());
                    float x0 = sprite.sprite->images()->Get(sprite.imageIndex)->x0() / (float)tex->size.x;
                    float x1 = sprite.sprite->images()->Get(sprite.imageIndex)->x1() / (float)tex->size.x;
                    float y0 = sprite.sprite->images()->Get(sprite.imageIndex)->y0() / (float)tex->size.y;
                    float y1 = sprite.sprite->images()->Get(sprite.imageIndex)->y1() / (float)tex->size.y;
                    glm::vec2 origin = {sprite.sprite->origin()->x(), sprite.sprite->origin()->y()};
                    glm::vec2 size = {sprite.sprite->size()->x(), sprite.sprite->size()->y()};
                    g2d->spriteBatch->draw(*tex, transform.pos - origin, size, {x0, y0, x1, y1});

                    sprite.imageIndex++;
                    if (sprite.imageIndex >= sprite.sprite->images()->size()) {
                        sprite.imageIndex = 0;
                    }
                }
            }

        }
    }

    {
        auto chunks = ecs.query(ecs::Query().all<gear::SpriteComponent, DestroyOnAnimationEnd>());
        for (auto c: chunks) {
            auto chunk = ecs::ChunkView<ecs::EntityRef, gear::SpriteComponent>(*c);
            for (auto[entity, sprite] : chunk) {
                if (sprite.imageIndex == 0) {
                    cmd.destroyEntity(entity);
                }
            }
        }
    }

    g2d->spriteBatch->flush();
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



void gear::renderTextSystem(G2DInstance *g2d, gear::ecs::Registry &ecs, const gear::assets::Shader *shader) {

    foreachView(ecs, [&](View& view) {
        auto chunks = ecs.query(ecs::Query().all<Text, gear::Transform>());
        for(auto c : chunks) {
            for (auto[transform, text] : ecs::ChunkView<gear::Transform, Text>(*c)) {
                renderText(g2d, text.text, text.font, transform.pos, shader, view);
            }
        }
    });



}
