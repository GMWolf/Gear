//
// Created by felix on 05/01/2020.
//

#include <gear/Shader.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <iostream>
#include <generated/shader_generated.h>

gear::Shader::Shader(const char* vs, const char* fs){
    init(&vs, 1, &fs, 1);
}

static bool checkShaderCompilation(GLuint shader) {
    GLint isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::cerr << errorLog.data() << std::endl;
    }
    return isCompiled;
}

GLuint createShader(GLenum type, const char** source, int count) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, count, source, nullptr);
    glCompileShader(shader);

    if (!checkShaderCompilation(shader)) {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void gear::Shader::init(const char **vs, int vsCount, const char **fs, int fsCount) {

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vs, vsCount);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fs, fsCount);

    if (!vertexShader || !fragmentShader) {
        return;
    }

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

void gear::Shader::use() const{
    glUseProgram(program);
}

GLuint gear::Shader::uniformLocation(const char *name) const {
    return glGetUniformLocation(program, name);
}

gear::Shader::Shader(gear::Shader &&o) noexcept : program(o.program){
    o.program = 0;
}

gear::Shader &gear::Shader::operator=(gear::Shader &&o) noexcept {
    glDeleteProgram(program);
    program = o.program;
    o.program = 0;
    return *this;
}


gear::Shader gear::ShaderLoader::load(const gear::assets::Shader* shader, AssetRegistry& registry) {
    return Shader(shader->vertex()->c_str(), shader->fragment()->c_str());
}
