//
// Created by felix on 01/09/2020.
//

#include <g2d.h>
#include "SpriteBatch.h"
#include "Shader.h"
#include "Texture.h"
#include "PrimDraw.h"
#include "FontCache.h"
#include <glad/glad.h>

namespace gear {

    const Gapi* g2dGetGapi()
    {
        static Gapi g2dGapi {
                .api = Gapi::Ogl{
                        .version_major = 3,
                        .version_minor = 3,
                        .loadGLLoader = gladLoadGLLoader,
                }
        };

        return &g2dGapi;
    }


    G2DInstance::G2DInstance(const G2DInstanceCreateInfo &createInfo) {
        spriteBatch = new SpriteBatch(*createInfo.spriteBatchCreateInfo);
        shaderStore = new ShaderStore();
        textureStore = new TextureStore();
        primDraw = new PrimDraw();
        fontCache = new FontCache(1024);
    }

    G2DInstance::~G2DInstance() {
        delete spriteBatch;
        delete shaderStore;
        delete textureStore;
        delete primDraw;
        delete fontCache;
    }

    void G2DInstance::flush() {
        spriteBatch->flush();
    }

    void G2DInstance::clearColor(const glm::vec4 &color) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void G2DInstance::setBlendMode() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void G2DInstance::setCullFace(bool enable) {
        if (enable) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    void G2DInstance::setViewport(const glm::vec2 &pos, const glm::vec2 &size) {
        glViewport(pos.x, pos.y, size.x, size.y);
    }
}