//
// Created by felix on 05/01/2020.
//

#ifndef GEAR_TEXTURE_H
#define GEAR_TEXTURE_H

#include <glad/glad.h>
#include <string>


namespace Gear {

    class Texture {
    public:
        explicit Texture(const std::string& name);

        GLuint tex{0};
    private:
        int width{0};
        int height{0};
    };


}


#endif //GEAR_TEXTURE_H
