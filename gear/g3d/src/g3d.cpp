//
// Created by felix on 31/10/2020.
//
#include <g3d.h>
#include <glad/glad.h>
#include "Texture.h"

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

    G3DInstance::G3DInstance() = default;

    void G3DInstance::debugTexture(const assets::Texture *texture) {
        const char* vertText = "out vec2 texCoord;\n"
                               " \n"
                               "void main()\n"
                               "{\n"
                               "    float x = -1.0 + float((gl_VertexID & 1) << 2);\n"
                               "    float y = -1.0 + float((gl_VertexID & 2) << 1);\n"
                               "    texCoord.x = (x+1.0)*0.5;\n"
                               "    texCoord.y = (y+1.0)*0.5;\n"
                               "    gl_Position = vec4(x, y, 0, 1);\n"
                               "}";




        glDrawArrays( GL_TRIANGLES, 0, 3 );
    }
}