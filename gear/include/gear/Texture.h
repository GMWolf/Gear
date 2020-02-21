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
#include <vector>

namespace gear {

    class Texture {
    public:
        explicit Texture(const std::string& name);
        explicit Texture(glm::vec4 color);
        ~Texture();
        GLuint tex {0};
        glm::ivec2 size {0,0};
    };

    struct TexRegion {
        glm::vec4 uvs; // min uv, max uv
        glm::vec4 crop; //left, top, right, bottom
    };

    struct Sprite {
        glm::vec2 size {};
        glm::vec2 origin {};
        std::vector<TexRegion> texRegions {}; //TODO get rid of this allocation
        std::weak_ptr<const Texture> tex;
        uint16_t imageIndex {0};
    };
}


#endif //GEAR_TEXTURE_H
