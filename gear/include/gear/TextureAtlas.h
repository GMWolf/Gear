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

    class TextureAtlasLoader : public AssetLoader<TextureAtlas> {
        TextureAtlas load(const std::string &name, AssetRegistry& registry) override;
    };
}

#endif //GEAR_TEXTUREATLAS_H
