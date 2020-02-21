//
// Created by felix on 06/01/2020.
//

#ifndef GEAR_TEXTUREPACKER_H
#define GEAR_TEXTUREPACKER_H

#include <string>
#include <vector>
#include <filesystem>

namespace gear::texture_pack {

    struct Pixel {
        unsigned char c[4];
    };


    struct SubImage {
        unsigned short x, y;
        unsigned short width, height;
        const Pixel* data;
        bool packed = false;
    };

    struct Sprite {
        std::string name;
        std::vector<SubImage> images;
    };


    struct SpriteDescriptor {
        std::string name;
        std::vector<std::filesystem::path> images;
    };

    void packSprites(std::vector<Sprite>& sprites, unsigned short width, unsigned short height);

    void writeSprites(const std::vector<Sprite>& sprites, unsigned short width, unsigned short height, Pixel* data);

    void writePageTexture(const std::string& file, int width, int height, const Pixel * data);

    SpriteDescriptor getDescriptorFromPath(const std::filesystem::path& path);

    Sprite loadSprite(const SpriteDescriptor& desc);

    void freeSprite(Sprite& sprite);
}






#endif //GEAR_TEXTUREPACKER_H
