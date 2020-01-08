//
// Created by felix on 05/01/2020.
//

#ifndef GEAR_TEXTURE_H
#define GEAR_TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace gear {

    class Texture {
    public:
        explicit Texture(const std::string& name);
        explicit Texture(glm::vec4 color);
        ~Texture();
        GLuint tex {0};
        glm::ivec2 size {0,0};
    };

    struct Sprite {
        glm::vec2 size;
        glm::vec4 uv;
        std::weak_ptr<const Texture> tex;
    };

}


#endif //GEAR_TEXTURE_H
