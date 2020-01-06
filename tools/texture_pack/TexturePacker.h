//
// Created by felix on 06/01/2020.
//

#ifndef GEAR_TEXTUREPACKER_H
#define GEAR_TEXTUREPACKER_H

#include <string>

namespace gear::texture_pack {

    struct Pixel {
        unsigned char c[4];
    };

    struct Sprite {
        std::string name;
        unsigned short x, y;
        unsigned short width, height;
        const Pixel* data;
        bool packed = false;
    };


    void packSprites(Sprite* sprites, size_t count, unsigned short width, unsigned short height);


    void writeSprites(Sprite* sprite, size_t count, unsigned short width, unsigned short height, Pixel* data);
}






#endif //GEAR_TEXTUREPACKER_H
