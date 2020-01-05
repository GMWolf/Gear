//
// Created by felix on 05/01/2020.
//

#include <gear/Shader.h>

gear::Shader::Shader(const std::string &vs, const std::string &fs) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = vs.c_str();
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = fs.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);


    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);


    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

gear::Shader::~Shader() {
    glDeleteProgram(program);
}

void gear::Shader::use() {
    glUseProgram(program);
}
