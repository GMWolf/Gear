//
// Created by felix on 09/11/2020.
//
#include "DrawScene.h"
#include <gear/g3d/g3d.h>
#include <gear/ecs/ECS.h>
#include <gear/View.h>
#include <gear/Transform.h>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Remotery.h"
#include "Buffer.h"


namespace gear::g3d {


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

    struct CameraCullData {
        Transform3 cameraInverse;
        glm::vec2 sphereFactor;
        float tang;
        float aspect;
        float near;
        float far;
    };


    static CameraCullData buildCameraCullData(const Transform3& cameraTransform, const Camera& camera)
    {
        CameraCullData data;
        data.cameraInverse = cameraTransform.inverse();
        data.tang = tanf(camera.fov * 0.5f);
        data.aspect = camera.viewPort.size.x / (float) camera.viewPort.size.y;
        float fovx = atanf(data.tang * data.aspect);
        data.sphereFactor.x = 1.0f / cosf(fovx);
        data.sphereFactor.y = 1.0f / cosf(camera.fov * 0.5f);
        data.near = camera.near;
        data.far = camera.far;
        return data;
    }

    static bool meshletInView(const CameraCullData& cc, const assets::MeshletBounds* bounds, Transform3& transform) {

       // rmt_ScopedCPUSample(MeshletInView, RMTSF_Aggregate);

        glm::vec3 pos = transform.apply({bounds->center().x(), bounds->center().y(), bounds->center().z()});
        float radius = bounds->radius() * transform.scale;
        pos = cc.cameraInverse.apply(pos);

        if (-pos.z > cc.far + radius || -pos.z < cc.near - radius) {
            return false;
        }

        glm::vec2 d = cc.sphereFactor * radius;

        float az = -pos.z * cc.tang;
        if (pos.y > az + d.y || pos.y < -az-d.y) {
            return false;
        }

        az *= cc.aspect;
        if (pos.x > az+d.x || pos.x < -az-d.x) {
            return false;
        }

        return true;
    }


    struct MeshletBatch {
        static const size_t capacity = 1024 * 4;
        size_t drawCount;
        GLsizei indexCount[capacity];
        const void* indices[capacity];
        GLint baseVertex[capacity];
        g3d::RingBuffer::Block transformBlock;
        Transform3* transforms;
    };

    static void batchAppendMeshlet(MeshletBatch& batch, const assets::MeshletBuffer* meshletBuffer, size_t index, size_t indexOffset, size_t baseVertex, const Transform3& transform) {

        batch.indexCount[batch.drawCount] = meshletBuffer->indexCounts()->Get(index);
        batch.baseVertex[batch.drawCount] = meshletBuffer->vertexOffsets()->Get(index) + baseVertex;
        batch.indices[batch.drawCount] = (const void*)(uint64_t)(meshletBuffer->indexOffsets()->Get(index) + indexOffset);

        batch.transforms[batch.drawCount] = transform;
        batch.drawCount++;
    }


    static void submitMehsletBatch(G3DInstance& g3d, MeshletBatch& batch) {

        rmt_ScopedOpenGLSample(SubmitMeshlets);

        if (batch.drawCount > 0) {

            g3d.batchBuffers->transformBuffer.bind(GL_SHADER_STORAGE_BUFFER, 1, batch.transformBlock);

            glMultiDrawElementsBaseVertex(
                    GL_TRIANGLES,
                    batch.indexCount,
                    GL_UNSIGNED_BYTE,
                    batch.indices,
                    batch.drawCount,
                    batch.baseVertex
            );

            *batch.transformBlock.sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

            batch.drawCount = 0;
        }

    }


    static MeshletBatch newMeshletBatch(G3DInstance& g3d) {

        MeshletBatch batch{};
        batch.drawCount = 0;
        batch.transformBlock = g3d.batchBuffers->transformBuffer.allocateBlock(MeshletBatch::capacity * sizeof(Transform3), 0);
        batch.transforms = static_cast<Transform3 *>(batch.transformBlock.ptr);
        return batch;
    }


    static void renderSceneCamera(G3DInstance &g3d, ecs::Registry &registry, Camera &camera,
                                  Transform3 &cameraTransform) {

        rmt_ScopedCPUSample(RenderScene, 0);

        glViewport(camera.viewPort.pos.x, camera.viewPort.pos.y, camera.viewPort.size.x, camera.viewPort.size.y);

        glBindVertexArray(g3d.meshCache->meshletVAO);

        GLsync* cameraUniformSync;
        {
            g3d.batchBuffers->cameraBuffer->cameraTransform = cameraTransform;
            float aspect = camera.viewPort.size.x / (float) camera.viewPort.size.y;
            g3d.batchBuffers->cameraBuffer->projection = glm::perspective(camera.fov, aspect, camera.near, camera.far);
            g3d.batchBuffers->cameraBuffer.bind(0);
            cameraUniformSync = g3d.batchBuffers->cameraBuffer.end();
        }

        CameraCullData cc = buildCameraCullData(cameraTransform, camera);

        auto meshQuery = registry.query().all<Transform3, MeshInstance>();

        //temp only bind material for very first instance
        {
            if (meshQuery.begin() != meshQuery.end()) {
                auto c = *meshQuery.begin();
                auto *meshInstance = static_cast<MeshInstance *>(c->get(ecs::Component<MeshInstance>::ID(), 0));
                const auto &shader = g3d.shaderCache->get(meshInstance->shader);
                const auto &mesh = g3d.meshCache->get(meshInstance->mesh);

                glUseProgram(shader.id);
                TextureBindings textureBindings{};
                textureBindings.albedo = getShaderBinding(shader, "albedo");
                textureBindings.occlusion = getShaderBinding(shader, "occlusion");
                textureBindings.normal = getShaderBinding(shader, "normalMap");
                textureBindings.metallicRoughness = getShaderBinding(shader, "metallicRoughness");

                bindMaterial((const assets::Material *) meshInstance->mesh->primitives()->Get(0)->material()->ptr(),
                             *g3d.textureCache, textureBindings);
            }
        }

        MeshletBatch batch = newMeshletBatch(g3d);

        for(auto meshChunk : meshQuery) {
            for(auto [transform, meshInstance] : ecs::ChunkView<Transform3, MeshInstance>(*meshChunk)) {

                const auto& mesh = g3d.meshCache->get(meshInstance.mesh);

                for(const auto& prim : *meshInstance.mesh->primitives()) {
                    auto primThing = g3d.meshCache->getMeshletPrimitive(prim);

                    for(int i = 0; i < prim->meshlets()->indexCounts()->size(); i++) {
                        if (meshletInView(cc, prim->meshlets()->bounds()->Get(i), transform)) {
                            batchAppendMeshlet(batch, prim->meshlets(), i, primThing.indexOffset,
                                             primThing.baseVertex, transform);
                            if (batch.drawCount == gear::g3d::MeshletBatch::capacity) {
                                submitMehsletBatch(g3d, batch);
                                batch = newMeshletBatch(g3d);
                            }
                        }
                    }

                }

            }
        }

        if (batch.drawCount) {
            submitMehsletBatch(g3d, batch);
        }

        *cameraUniformSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }
}

namespace gear {
    void G3DInstance::renderScene(ecs::Registry &registry) {

        glEnable(GL_DEPTH_TEST);

        auto cameraQuery = registry.query().all<Transform3, Camera>();

        for (auto cameraChunk : cameraQuery) {
            for (auto [transform, camera] : ecs::ChunkView<Transform3, Camera>(*cameraChunk)) {
                g3d::renderSceneCamera(*this, registry, camera, transform);
            }
        }
    }
}