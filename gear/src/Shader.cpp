//
// Created by felix on 05/01/2020.
//

#include <gear/Shader.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <iostream>

gear::Shader::Shader(const std::string &path) {

    std::ifstream t(path);
    std::string source((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());

    const char* versionString = "#version 330 core\n";
    const char* shaderData = source.c_str();
    const char* vs[]{
        versionString,
        "#define VERTEX_SHADER\n\n",
        shaderData
    };
    const char* fs[]{
        versionString,
        "#define FRAGMENT_SHADER\n\n",
        shaderData
    };

    init(vs, 3, fs, 3);
}

gear::Shader::Shader(const std::string &vs, const std::string &fs){
    const char* vsPtr = vs.c_str();
    const char* fsPtr = fs.c_str();
    init(&vsPtr, 1, &fsPtr, 1);
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

void gear::Shader::use() {
    glUseProgram(program);
}


gear::AssetEntry gear::ShaderLoader::load(const std::string &name) {
    return AssetEntry{
        std::make_shared<Shader>(name)
    };
}
