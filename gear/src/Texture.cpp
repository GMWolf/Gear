//
// Created by felix on 05/01/2020.
//

#include <gear/Texture.h>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

gear::Texture::Texture(gear::Texture && o) noexcept : size(o.size), tex(o.tex) {
    o.size = {0,0};
    o.tex = 0;
}

gear::Texture &gear::Texture::operator=(Texture&& o) noexcept {
    glDeleteTextures(1, &tex);
    tex = o.tex;
    size = o.size;

    o.tex = 0;
    o.size = {0,0};

    return *this;
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

gear::Texture::Texture(GLuint tex, glm::ivec2 size) : tex(tex), size(size){
}


gear::Texture gear::TextureLoader::load(const std::string &name, gear::AssetRegistry &registry) {
    int x, y, n;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(name.c_str(), &x, &y, &n, 4);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return Texture(tex, {x, y});
}
