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
        Transform3 cameraTransform;
        Transform3 transform;
        glm::mat4 projection;
    };

    struct TextureBindings {
        int albedo;
        int occlusion;
        int normal;
        int metallicRoughness;
    };

    static void bindTexture(int binding, const assets::Texture* tex, g3d::TextureCache& textureCache) {
        if (binding >= 0) {
            glBindTextureUnit(binding, textureCache.get(tex).id);
        }
    }

    static int getShaderBinding(const g3d::Shader& shd, const char* name) {
        auto resource = shd.shaderDef->resources()->LookupByKey(name);
        if (resource) {
            return resource->binding();
        }
        return -1;
    }

    static void bindMaterial(const assets::Material* material, g3d::TextureCache& textureCache, const TextureBindings& textureBindings) {
        auto albedoTex = (const assets::Texture *)material->baseColor()->ptr();
        bindTexture(textureBindings.albedo, albedoTex, textureCache);
        auto occlusionTex = (const assets::Texture *)material->AmbientOcclusion()->ptr();
        bindTexture(textureBindings.occlusion, occlusionTex, textureCache);
        auto normalTex = (const assets::Texture *)material->normal()->ptr();
        bindTexture(textureBindings.normal, normalTex, textureCache);
        auto metalRoughnessTex = (const assets::Texture *)material->MetallicRoughness()->ptr();
        bindTexture(textureBindings.metallicRoughness, metalRoughnessTex, textureCache);
    }


    struct MultiDrawCmdBuffer {
        static const size_t capacity = 1024;
        size_t drawCount;

        GLsizei indexCount[capacity];
        const void* indices[capacity];
        GLint baseVertex[capacity];
    };




    bool meshletInView(const Transform3& cameraTransform, const Camera& camera, const assets::MeshletBounds* bounds, Transform3& transform) {

        glm::vec3 pos = transform.apply({bounds->center().x(), bounds->center().y(), bounds->center().z()});
        float radius = bounds->radius() * transform.scale;
        pos = cameraTransform.inverse().apply(pos);

        float tang = tanf(camera.fov * 0.5f);
        float aspect = camera.viewPort.size.x / (float) camera.viewPort.size.y;
        glm::vec2 sphereFactor;
        float fovx = atanf(tang*aspect);
        sphereFactor.x = 1.0f / cosf( fovx );
        sphereFactor.y = 1.0f / cosf(camera.fov * 0.5f);

        /*
        glm::vec3 v = pos - cameraTransform.position;
        float pcz = -pos.z;//glm::dot(v, -cameraBasisZ);
        if (pcz > camera.far || pcz < camera.near) {
            return false;
        }

        float pcy = pos.y;//glm::dot(v, cameraBasisY);
        float aux = pcz * tang;
        if (pcy > aux || pcy < -aux) {
            return false;
        }

        float pcx = pos.x;//glm::dot(v, cameraBasisX);
        aux = aux * aspect;
        if (pcx > aux || pcx < -aux) {
            return false;
        }

        return true;
         */


        //float height = camera.near * tang;
        //float width = height * aspect;

        if (-pos.z > camera.far + radius || -pos.z < camera.near - radius) {
            return false;
        }

        glm::vec2 d = sphereFactor * radius;

        float az = -pos.z * tang;
        if (pos.y > az + d.y || pos.y < -az-d.y) {
            return false;
        }

        az *= aspect;
        if (pos.x > az+d.x || pos.x < -az-d.x) {
            return false;
        }


        return true;

    }

    static void appendMeshletCmd(MultiDrawCmdBuffer& cmds, const assets::MeshletBuffer* meshletBuffer, size_t index, size_t indexOffset, size_t baseVertex) {

        cmds.indexCount[cmds.drawCount] = meshletBuffer->indexCounts()->Get(index);
        cmds.baseVertex[cmds.drawCount] = meshletBuffer->vertexOffsets()->Get(index) + baseVertex;
        cmds.indices[cmds.drawCount] = (const void*)(uint64_t)(meshletBuffer->indexOffsets()->Get(index) + indexOffset);

        cmds.drawCount++;
    }


    static void submitMeshletsCmds(MultiDrawCmdBuffer& cmds) {

        glMultiDrawElementsBaseVertex(
                GL_TRIANGLES,
                cmds.indexCount,
                GL_UNSIGNED_BYTE,
                cmds.indices,
                cmds.drawCount,
                cmds.baseVertex
                );

    }

    static void renderSceneCamera(G3DInstance &g3d, ecs::Registry &registry, Camera &camera,
                                  Transform3 &cameraTransform) {

        glViewport(camera.viewPort.pos.x, camera.viewPort.pos.y, camera.viewPort.size.x, camera.viewPort.size.y);

        static GLuint ubo = 0;
        if (ubo == 0) {
            glCreateBuffers(1, &ubo);
            glNamedBufferStorage(ubo, sizeof(SceneBuffer), nullptr,  GL_MAP_WRITE_BIT);
        }


        //glBindVertexArray(g3d.meshCache->vao);
        glBindVertexArray(g3d.meshCache->meshletVAO);

        auto meshQuery = registry.query().all<Transform3, MeshInstance>();
        for(auto meshChunk : meshQuery) {
            for(auto [transform, meshInstance] : ecs::ChunkView<Transform3, MeshInstance>(*meshChunk)) {

                const auto& shader = g3d.shaderCache->get(meshInstance.shader);
                const auto& mesh = g3d.meshCache->get(meshInstance.mesh);

                glUseProgram(shader.id);
                TextureBindings textureBindings{};
                textureBindings.albedo = getShaderBinding(shader, "albedo");
                textureBindings.occlusion = getShaderBinding(shader, "occlusion");
                textureBindings.normal = getShaderBinding(shader, "normalMap");
                textureBindings.metallicRoughness = getShaderBinding(shader, "metallicRoughness");

                { // update scene buffer
                    auto sceneBuffer = static_cast<SceneBuffer *>(glMapNamedBuffer(ubo, GL_WRITE_ONLY));
                    sceneBuffer->cameraTransform = cameraTransform;
                    sceneBuffer->transform = transform;
                    float aspect = camera.viewPort.size.x / (float) camera.viewPort.size.y;
                    sceneBuffer->projection = glm::perspective(camera.fov, aspect, camera.near, camera.far);
                    glUnmapNamedBuffer(ubo);
                }
                { //set scene buffer
                    auto binding = shader.shaderDef->resources()->LookupByKey("SceneBuffer")->binding();
                    glBindBufferBase(GL_UNIFORM_BUFFER, binding, ubo);
                }


                MultiDrawCmdBuffer multiDrawCmdBuffer;

                for(const auto& prim : *meshInstance.mesh->primitives()) {
                    bindMaterial((const assets::Material*)prim->material()->ptr(), *g3d.textureCache, textureBindings);

                    auto primThing = g3d.meshCache->getMeshletPrimitive(prim);
                    multiDrawCmdBuffer.drawCount = 0;
                    for(int i = 0; i < prim->meshlets()->indexCounts()->size(); i++) {
                        if (meshletInView(cameraTransform, camera, prim->meshlets()->bounds()->Get(i), transform)) {
                            appendMeshletCmd(multiDrawCmdBuffer, prim->meshlets(), i, primThing.indexOffset,
                                             primThing.baseVertex);
                        }
                    }
                    submitMeshletsCmds(multiDrawCmdBuffer);
                }

                //for(const auto& prim : mesh.primitives) {
                //    bindMaterial(prim.material, *g3d.textureCache, textureBindings);
                //    //glDrawElementsBaseVertex(GL_TRIANGLES, prim.indexCount, GL_UNSIGNED_SHORT, (void*)prim.indexOffset, prim.baseVertex);
                //}

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