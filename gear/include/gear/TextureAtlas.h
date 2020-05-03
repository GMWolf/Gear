//
// Created by felix on 07/01/2020.
//

#ifndef GEAR_TEXTUREATLAS_H
#define GEAR_TEXTUREATLAS_H

#include <string>
#include <memory>
#include <unordered_map>
#include "Texture.h"
#include "AssetManager.h"

namespace gear {

    class TextureAtlas {
    public:
        Sprite getSprite(const std::string& name) const;

    private:
        AssetReference<Texture> texture;
        std::unordered_map<std::string, Sprite> sprites;
        friend class TextureAtlasLoader;
    };

    namespace assets {
        class Atlas;
    }

    class TextureAtlasLoader{
    public:
        static TextureAtlas load(const assets::Atlas* atlas, AssetRegistry& registry);
    };
}

#endif //GEAR_TEXTUREATLAS_H
