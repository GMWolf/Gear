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


    class ShaderLoader : public AssetLoader<Shader> {
    public:
        Shader load(const std::string &name, AssetRegistry& registry) override;
    };

}

#endif //GEAR_SHADER_H
