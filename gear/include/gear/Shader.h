//
// Created by felix on 05/01/2020.
//

#ifndef GEAR_SHADER_H
#define GEAR_SHADER_H

#include <glad/glad.h>
#include <string>
#include "AssetManager.h"

namespace gear {

    class Shader {
        void init(const char** vs, int vsCount, const char** fs, int fsCount);
    public:
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&& o) noexcept;
        Shader& operator=(Shader&& o) noexcept;
        Shader(const std::string& vs, const std::string& fs);

        ~Shader();
        void use() const;

        GLuint uniformLocation(const char* name) const;


    private:
        GLuint program{};
    };

    namespace assets {
        class Shader;
    }

    class ShaderLoader {
    public:
        static Shader load(const assets::Shader* shaderDef, AssetRegistry& registry);
    };

}

#endif //GEAR_SHADER_H
