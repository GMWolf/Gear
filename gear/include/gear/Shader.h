//
// Created by felix on 05/01/2020.
//

#ifndef GEAR_SHADER_H
#define GEAR_SHADER_H

#include <glad/glad.h>
#include <string>

namespace gear {

    class Shader {
    public:
        Shader(const std::string& vs, const std::string& fs);
        ~Shader();
        void use();

        GLuint uniformLocation(const char* name) {
            return glGetUniformLocation(program, name);
        }



    private:
        GLuint program{};
    };

}

#endif //GEAR_SHADER_H
