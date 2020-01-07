//
// Created by felix on 06/01/2020.
//

#include "TexturePacker.h"
#include <vector>
#include <stb_rect_pack.h>
#include <stb_image.h>
#include <nlohmann/json.hpp>

void gear::texture_pack::packSprites(gear::texture_pack::Sprite *sprites, size_t count,
                                     unsigned short width, unsigned short height) {

    std::vector<stbrp_rect> rects(count);

    for(int i = 0; i < count; i++) {
        rects[i] = {i, sprites[i].width, sprites[i].height};
    }

    int numNodes = width;
    std::vector<stbrp_node> nodes(numNodes);

    stbrp_context ctx;
    stbrp_init_target(&ctx, width, height, nodes.data(), numNodes);

    stbrp_pack_rects(&ctx, rects.data(), rects.size());

    for(auto& rect : rects) {
        if (rect.was_packed) {
            sprites[rect.id].x = rect.x;
            sprites[rect.id].y = rect.y;
            sprites[rect.id].packed = true;
        }
    }
}

static void writeSprite(gear::texture_pack::Sprite& sprite, unsigned short width, unsigned short height,
        gear::texture_pack::Pixel *data) {

    for(int iy = 0; iy < sprite.height; iy++) {
        for(int ix = 0; ix < sprite.width; ix++) {
            int px = sprite.x + ix;
            int py = sprite.y + iy;

            data[px + py * width] = sprite.data[ix + iy * sprite.width];
        }
    }

}

void gear::texture_pack::writeSprites(gear::texture_pack::Sprite *sprite, size_t count, unsigned short width,
                                      unsigned short height, gear::texture_pack::Pixel *data) {
    for(int i = 0; i < count; i++) {
        if (sprite[i].packed) {
            writeSprite(sprite[i], width, height, data);
        }
    }
}