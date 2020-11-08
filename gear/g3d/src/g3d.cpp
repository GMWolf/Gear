//
// Created by felix on 31/10/2020.
//
#include <g3d.h>
#include <glad/glad.h>
#include <iostream>
#include "Texture.h"
#include "Shader.h"

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
    }
    G3DInstance::~G3DInstance() = default;


    void G3DInstance::debugTexture(const assets::Texture *texture, const assets::Shader *shader) {

        static GLuint vao = 0;
        if (vao == 0) {
            glCreateVertexArrays(1, &vao);
        }

        glViewport(0, 0, 720, 720);
        glBindVertexArray(vao);
        glUseProgram(shaderCache->get(shader).id);


        glDrawArrays( GL_TRIANGLES, 0, 3 );
    }

}