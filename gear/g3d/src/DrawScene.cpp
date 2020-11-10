//
// Created by felix on 09/11/2020.
//
#include <gear/g3d/g3d.h>
#include <gear/ecs/ECS.h>
#include <gear/View.h>
#include <gear/Transform.h>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

namespace gear {


    struct SceneBuffer {
        Transform3 transform;
        glm::mat4 projection;
    };

    static void renderSceneCamera(G3DInstance &g3d, ecs::Registry &registry, Camera &camera,
                                  Transform3 &cameraTransform) {

        glViewport(camera.viewPort.pos.x, camera.viewPort.pos.y, camera.viewPort.size.x, camera.viewPort.size.y);

        static GLuint ubo = 0;
        if (ubo == 0) {
            glCreateBuffers(1, &ubo);
            glNamedBufferStorage(ubo, sizeof(SceneBuffer), nullptr,  GL_MAP_WRITE_BIT);
        }


        glBindVertexArray(g3d.meshCache->vao);

        auto meshQuery = registry.query().all<Transform3, MeshInstance>();
        for(auto meshChunk : meshQuery) {

            for(auto [transform, meshInstance] : ecs::ChunkView<Transform3, MeshInstance>(*meshChunk)) {

                const auto& shader = g3d.shaderCache->get(meshInstance.shader);
                const auto& texture = g3d.textureCache->get(meshInstance.texture);
                const auto& mesh = g3d.meshCache->get(meshInstance.mesh);

                glUseProgram(shader.id);
                { // set texture
                    auto binding = shader.shaderDef->resources()->LookupByKey("tex")->binding();
                    glBindTextureUnit(binding, texture.id);
                }

                { // update scene buffer
                    auto sceneBuffer = static_cast<SceneBuffer *>(glMapNamedBuffer(ubo, GL_WRITE_ONLY));
                    sceneBuffer->transform = cameraTransform.inverse().apply(transform);
                    float aspect = camera.viewPort.size.x / (float) camera.viewPort.size.y;
                    sceneBuffer->projection = glm::perspective(camera.fov, aspect, camera.near, camera.far);
                    glUnmapNamedBuffer(ubo);
                }
                { //set scene buffer
                    auto binding = shader.shaderDef->resources()->LookupByKey("SceneBuffer")->binding();
                    glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
                }

                for(const auto& prim : mesh.primitives) {
                    glDrawElements(GL_TRIANGLES, prim.indexCount, GL_UNSIGNED_INT, nullptr);
                }

            }
        }

    }

    void G3DInstance::renderScene(ecs::Registry &registry) {

        glEnable(GL_DEPTH_TEST);

        auto cameraQuery = registry.query().all<Transform3, Camera>();

        for (auto cameraChunk : cameraQuery) {
            for (auto [transform, camera] : ecs::ChunkView<Transform3, Camera>(*cameraChunk)) {
                renderSceneCamera(*this, registry, camera, transform);
            }
        }
    }
}