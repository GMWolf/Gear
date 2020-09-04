//
// Created by felix on 29/02/2020.
//

#ifndef GEAR_ASSETS_H
#define GEAR_ASSETS_H

#include <memory>
#include <flatbuffers/hash.h>

namespace gear {

    namespace assets{
        class Bundle;
        class Texture;
        class Sprite;
        class BitmapFont;
        class Shader;
        class TileSet;
        class Map;
    }

    class AssetRegistry {
        class Store;
        std::unique_ptr<Store> store;
    public:
        AssetRegistry();
        AssetRegistry(const AssetRegistry&) = delete;
        AssetRegistry& operator=(const AssetRegistry&) = delete;
        ~AssetRegistry();

        const assets::Texture* getTexture(uint64_t name);
        const assets::Texture* getTexture(const char* name);
        const assets::Sprite* getSprite(uint64_t name);
        const assets::Sprite* getSprite(const char* name);
        const assets::BitmapFont* getBitmapFont(uint64_t name);
        const assets::BitmapFont* getBitmapFont(const char* name);
        const assets::Shader* getShader(uint64_t name);
        const assets::Shader* getShader(const char* name);
        const assets::TileSet* getTileSet(uint64_t name);
        const assets::TileSet* getTileSet(const char* name);
        const assets::Map* getMap(uint64_t name);
        const assets::Map* getMap(const char* name);


        void loadBundle(const std::string& name);
        void registerBundleRecursive(const assets::Bundle* bundle);
        void patchPointers(const assets::Bundle* bundle);
    };



    inline const gear::assets::Texture *gear::AssetRegistry::getTexture(const char* name) {
        return getTexture(flatbuffers::HashFnv1<uint64_t>(name));
    }

    inline const gear::assets::Sprite* gear::AssetRegistry::getSprite(const char *name) {
        return getSprite(flatbuffers::HashFnv1<uint64_t>(name));
    }

    inline const gear::assets::BitmapFont* gear::AssetRegistry::getBitmapFont(const char *name) {
        return getBitmapFont(flatbuffers::HashFnv1<uint64_t>(name));
    }

    inline const gear::assets::Shader* gear::AssetRegistry::getShader(const char *name) {
        return getShader(flatbuffers::HashFnv1<uint64_t>(name));
    }

    inline const gear::assets::TileSet* gear::AssetRegistry::getTileSet(const char *name) {
        return getTileSet(flatbuffers::HashFnv1<uint64_t>(name));
    }

    inline const gear::assets::Map* gear::AssetRegistry::getMap(const char *name) {
        return getMap(flatbuffers::HashFnv1<uint64_t>(name));
    }


}

#endif //GEAR_ASSETS_H
