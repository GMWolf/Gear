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
        explicit TextureAtlas(const std::string& name);

        Sprite getSprite(const std::string& name) const;

    private:
        std::shared_ptr<Texture> texture;
        std::unordered_map<std::string, Sprite> sprites;
    };

    class TextureAtlasLoader : public AssetLoader {
    public:
        AssetEntry load(const std::string &name) override;
    };
}

#endif //GEAR_TEXTUREATLAS_H
