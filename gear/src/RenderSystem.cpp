//
// Created by felix on 01/04/2020.
//

#include <gear/RenderSystem.h>
#include <glm/gtc/type_ptr.hpp>

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
