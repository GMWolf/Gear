//
// Created by felix on 05/01/2020.
//

#ifndef GEAR_SHADER_H
#define GEAR_SHADER_H

#include <glad/glad.h>
#include <unordered_map>

namespace gear {

    class Shader {
        void init(const char** vs, int vsCount, const char** fs, int fsCount);
    public:
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&& o) noexcept;
        Shader& operator=(Shader&& o) noexcept;
        Shader(const char* vs, const char* fs);

        ~Shader();
        void use() const;

        GLuint uniformLocation(const char* name) const;

    private:
        GLuint program{};
    };

    namespace assets {
        class Shader;
    }

    Shader createShader(const assets::Shader* shaderDef);

    class ShaderStore {
        std::unordered_map<const assets::Shader*, Shader> store;
    public:

        Shader* getShader(const assets::Shader* shader);
    };

}

#endif //GEAR_SHADER_H
