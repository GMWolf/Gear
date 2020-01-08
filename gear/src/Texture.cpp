//
// Created by felix on 05/01/2020.
//

#include <gear/Texture.h>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

gear::Texture::Texture(const std::string &name) {
    int x, y, n;
    unsigned char* data = stbi_load(name.c_str(), &x, &y, &n, 4);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    size = {x, y};
}

gear::Texture::~Texture() {
    glDeleteTextures(1, &tex);
}

gear::Texture::Texture(glm::vec4 color) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_FLOAT, glm::value_ptr(color));

    glBindTexture(GL_TEXTURE_2D, 0);

    size = {1,1};
}

