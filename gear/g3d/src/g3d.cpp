//
// Created by felix on 31/10/2020.
//
#include <g3d.h>
#include <glad/glad.h>

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
}