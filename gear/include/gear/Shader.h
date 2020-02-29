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
        explicit Shader(const std::string& path);
        Shader(const std::string& vs, const std::string& fs);
        ~Shader();
        void use();

        GLuint uniformLocation(const char* name) {
            return glGetUniformLocation(program, name);
        }


    private:
        GLuint program{};
    };


    class ShaderLoader : public AssetLoader {
    public:
        AssetEntry load(const std::string &name) override;
    };

}

#endif //GEAR_SHADER_H
