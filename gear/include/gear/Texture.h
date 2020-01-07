//
// Created by felix on 05/01/2020.
//

#ifndef GEAR_TEXTURE_H
#define GEAR_TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <glm/vec2.hpp>


namespace gear {

    class Texture {
    public:
        explicit Texture(const std::string& name);
        ~Texture();
        GLuint tex {0};
        glm::ivec2 size {0,0};
    };


}


#endif //GEAR_TEXTURE_H
