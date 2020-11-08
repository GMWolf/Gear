//
// Created by felix on 08/11/2020.
//

#ifndef GEAR_SHADER_H
#define GEAR_SHADER_H
#include <glad/glad.h>
#include "GlObject.h"
#include <gear/shader_generated.h>
#include <unordered_map>

namespace gear::g3d {

    class Shader : public GlObject<Shader> {
    public:
        explicit Shader(const assets::Shader* shaderDef);
        void release();
        const assets::Shader* shaderDef{nullptr};
    };

    class ShaderCache {
        std::unordered_map<const assets::Shader*, Shader> map;

    public:
        Shader& get(const assets::Shader*);
    };
}


#endif //GEAR_SHADER_H
