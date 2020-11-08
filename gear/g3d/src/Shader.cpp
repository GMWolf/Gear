//
// Created by felix on 08/11/2020.
//

#include <iostream>
#include "Shader.h"

void gear::g3d::Shader::release() {
    glDeleteProgram(id);
    id = 0;
}


static GLuint createShaderStage(GLenum shaderType, const uint32_t* data, size_t size) {

    GLuint shd = glCreateShader(shaderType);
    glShaderBinary(1, &shd, GL_SHADER_BINARY_FORMAT_SPIR_V, data, size * sizeof(uint32_t));
    glSpecializeShader(shd, "main", 0, nullptr, nullptr);

    GLint isCompiled;
    glGetShaderiv(shd, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        GLint maxLength = 0;
        glGetShaderiv(shd, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shd, maxLength, &maxLength, &infoLog[0]);
        std::cerr << infoLog.data() << "\n";
        glDeleteShader(shd);
        return 0;
    }

    return shd;
}

gear::g3d::Shader::Shader(const gear::assets::Shader *shaderDef) : GlObject(0), shaderDef(shaderDef){

    auto vert = createShaderStage(GL_VERTEX_SHADER, shaderDef->vertexText()->bin()->data(), shaderDef->vertexText()->bin()->size());
    auto frag = createShaderStage(GL_FRAGMENT_SHADER, shaderDef->fragmentText()->bin()->data(), shaderDef->fragmentText()->bin()->size());
    if (vert != 0 && frag != 0) {
        id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);

        glLinkProgram(id);
        GLint isLinked = 0;
        glGetProgramiv(id, GL_LINK_STATUS, (int *) &isLinked);
        if (isLinked == GL_FALSE) {
            std::cout << "error!" << std::endl;
            GLint maxLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);
            std::cerr << "error: " << maxLength << " " << infoLog.data() << "\n";
            glDeleteProgram(id);
            id = 0;
        }
    }


    if (id) {
        glDetachShader(id, vert);
        glDetachShader(id, frag);
    }
    glDeleteShader(vert);
    glDeleteShader(frag);

    std::cout << id << "\n";
}

gear::g3d::Shader &gear::g3d::ShaderCache::get(const gear::assets::Shader * shd) {
    auto it = map.find(shd);
    if(it == map.end()) {
        it = map.insert({shd, Shader(shd)}).first;
    }
    return it->second;
}
