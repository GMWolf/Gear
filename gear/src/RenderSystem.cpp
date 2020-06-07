//
// Created by felix on 01/04/2020.
//

#include <gear/RenderSystem.h>
#include <gear/ECS/ECS.h>
#include <gear/View.h>
#include <gear/CoreComponents.h>
#include <glm/gtc/type_ptr.hpp>
#include <gear/SpriteBatch.h>
#include <gear/PrimDraw.h>
#include <gear/Shader.h>

void gear::renderSprites(gear::ecs::Registry &ecs, gear::SpriteBatch &batch, const gear::Shader &shader) {

    using namespace gear;
    static const ecs::Query viewQuery = ecs::Query().all<View>();
    static const ecs::Query spriteQuery = ecs::Query().all<Sprite, Transform>();

    const size_t spriteChunkArraySize = 1024;
    ecs::Chunk* spriteChunkArray[spriteChunkArraySize];
    auto spriteChunks = ecs.queryChunks(spriteQuery, spriteChunkArray, spriteChunkArraySize);

    const size_t viewChunkArraySize = 1;
    ecs::Chunk* viewChunkArray[viewChunkArraySize];
    auto viewChunks = ecs.queryChunks(viewQuery, viewChunkArray, viewChunkArraySize);

    shader.use();
    glUniform1i(shader.uniformLocation("tex"), 0);

    for(auto c : viewChunks) {
        for(auto [view] : ecs::ChunkView<View>(*c)) {

            auto vm = view.matrix();
            glUniformMatrix4fv(shader.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

            for(auto c : spriteChunks) {
                for(auto [sprite, transform] : ecs::ChunkView<Sprite, Transform>(*c)) {
                    batch.draw(sprite, transform.pos);
                }
            }

        }
    }

    batch.flush();
}


void gear::renderDebugShapes(gear::ecs::Registry &ecs, gear::PrimDraw& primDraw, const gear::Shader &shader) {

    const size_t chunkArraySize = 1024;
    ecs::Chunk* chunkArray[chunkArraySize];
    {

        gear::View view {{0,0}, {480, 720}};

        shader.use();
        glUniform1i(shader.uniformLocation("tex"), 0);
        auto vm = view.matrix();
        glUniformMatrix4fv(shader.uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(vm));

        auto chunks = ecs.queryChunks(ecs::Query().all<gear::Transform, gear::CollisionShape>(), chunkArray,
                                      chunkArraySize);

        for(auto c : chunks) {
            auto chunk = ecs::ChunkView<gear::Transform, gear::CollisionShape>(*c);
            for(auto[transform, shape] : chunk) {
                if (auto circle = std::get_if<gear::Circle>(&shape)) {
                    primDraw.circle(transform.pos + circle->center, circle->radius);
                }
                if (auto rect = std::get_if<gear::Rectangle>(&shape)) {
                    primDraw.rect(transform.pos + rect->min, transform.pos + rect->max);
                }
            }
        }
    }

    primDraw.flush();
}
