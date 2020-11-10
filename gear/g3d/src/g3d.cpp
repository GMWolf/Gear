//
// Created by felix on 31/10/2020.
//
#include <g3d.h>
#include <glad/glad.h>
#include <iostream>
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"

namespace gear {
    const Gapi *g3dGetGapi() {
        static Gapi g2dGapi{
                .api = Gapi::Ogl{
                        .version_major = 3,
                        .version_minor = 3,
                        .loadGLLoader = gladLoadGLLoader,
                }
        };

        return &g2dGapi;
    }

    G3DInstance::G3DInstance() {
        shaderCache = std::make_unique<g3d::ShaderCache>();
        textureCache = std::make_unique<g3d::TextureCache>();
        meshCache = std::make_unique<g3d::MeshCache>();
    }
    G3DInstance::~G3DInstance() = default;


    void G3DInstance::debugTexture(const assets::Texture *texture, const assets::Shader *shader) {

        static GLuint vao = 0;
        if (vao == 0) {
            glCreateVertexArrays(1, &vao);
        }

        glViewport(0, 0, 720, 720);
        glBindVertexArray(vao);
        auto& shd = shaderCache->get(shader);
        glUseProgram(shd.id);
        auto bind = shd.shaderDef->resources()->LookupByKey("tex")->binding();
        glBindTextureUnit(bind, textureCache->get(texture).id);

        glDrawArrays( GL_TRIANGLES, 0, 3 );
    }

    void
    G3DInstance::debugMesh(const assets::Mesh *mesh, const assets::Texture *texture, const assets::Shader *shader) {
        const auto& m = meshCache->get(mesh);
        const auto& t = textureCache->get(texture);
        const auto& s = shaderCache->get(shader);

        glBindVertexArray(meshCache->vao);
        glUseProgram(s.id);
        auto loc = s.shaderDef->resources()->LookupByKey("tex")->binding();
        glBindTextureUnit(0, t.id);
        glProgramUniform1i(s.id, loc, 0);

        for(const auto& prim : m.primitives) {
            glDrawElements(GL_TRIANGLES, prim.indexCount, GL_UNSIGNED_SHORT, nullptr);
        }
    }

    void G3DInstance::clearBuffer(glm::vec4 color, float depth) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClearDepthf(depth);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }






}