//
// Created by felix on 06/01/2020.
//

#include "TexturePacker.h"
#include <vector>
#include <stb_rect_pack.h>
#include <stb_image.h>
#include <nlohmann/json.hpp>
#include <filesystem>

#include <stb_image.h>
#include <stb_image_write.h>

namespace fs = std::filesystem;

void gear::texture_pack::packSprites(std::vector<Sprite>& sprites, unsigned short width, unsigned short height) {

    std::vector<stbrp_rect> rects;
    rects.reserve(sprites.size());
    std::vector<SubImage*> subimages;
    subimages.reserve(sprites.size());

    {
        int id = 0;
        for(auto& spr : sprites) {
            for(auto& img : spr.images) {
                subimages.push_back(&img);
                rects.push_back({id, img.width, img.height});
                id++;
            }
        }
    }

    int numNodes = width;
    std::vector<stbrp_node> nodes(numNodes);

    stbrp_context ctx;
    stbrp_init_target(&ctx, width, height, nodes.data(), numNodes);

    stbrp_pack_rects(&ctx, rects.data(), rects.size());

    for(auto& rect : rects) {
        if (rect.was_packed) {
            subimages[rect.id]->x = rect.x;
            subimages[rect.id]->y = rect.y;
            subimages[rect.id]->packed = true;
        }
    }
}

static void writeImage(const gear::texture_pack::SubImage& image, unsigned short width, unsigned short height,
                        gear::texture_pack::Pixel *data) {

    for(int iy = 0; iy < image.height; iy++) {
        for(int ix = 0; ix < image.width; ix++) {
            int px = image.x + ix;
            int py = image.y + iy;

            data[px + py * width] = image.data[ix + iy * image.width];
        }
    }

}

void gear::texture_pack::writeSprites(const std::vector<Sprite>& sprites, unsigned short width,
                                     unsigned short height, gear::texture_pack::Pixel *data) {
    for(auto& spr : sprites) {
        for(auto& img : spr.images) {
            if (img.packed) {
                writeImage(img, width, height, data);
            }
        }
    }
}

gear::texture_pack::SpriteDescriptor gear::texture_pack::getDescriptorFromPath(const fs::path &path) {
    SpriteDescriptor desc;

    if (fs::is_directory(path)) {
        desc.name = path.filename();
        for(auto& sub : fs::directory_iterator(path)) {
            if (fs::is_regular_file(sub)) {
                desc.images.push_back(sub.path());
            }
        }
        std::sort(desc.images.begin(), desc.images.end());
    } else {
        desc.name = path.stem().filename().string();
        desc.images.push_back(path);
    }

    return desc;
}

gear::texture_pack::Sprite gear::texture_pack::loadSprite(const gear::texture_pack::SpriteDescriptor &desc) {

    Sprite sprite;
    sprite.name = desc.name;


    for(auto& path : desc.images) {
        int w, h, c;
        std::string pathStr = path.string();

        unsigned char* data = stbi_load(pathStr.c_str(), &w, &h, &c, 4);
        if (!data) exit(1);
        SubImage img = {
                0, 0,
                (unsigned short)w, (unsigned short)h,
                (Pixel*) data
        };
        sprite.images.push_back(img);

    }


    return sprite;
}

void gear::texture_pack::freeSprite(gear::texture_pack::Sprite &sprite) {
    for(auto& img : sprite.images) {
        stbi_image_free( (unsigned char*) img.data );
        img.data = nullptr;
    }
}

void gear::texture_pack::writePageTexture(const std::string &file, int width, int height,
                                          const gear::texture_pack::Pixel *data) {
    stbi_write_png(file.c_str(), width, height, 4, data, width * sizeof(Pixel));
}
