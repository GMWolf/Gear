//
// Created by felix on 29/02/2020.
//

#ifndef GEAR_ASSETMANAGER_H
#define GEAR_ASSETMANAGER_H


#include <memory>
#include <optional>
#include <string>


namespace gear {

    template<class T>
    class AssetEntry;

    template<class T>
    struct AssetReference {
        std::shared_ptr<AssetEntry<T>> ptr;

        bool pending() const;

        const T& get() const;

        const T* operator->() const;
        const T& operator*() const;

    };


    class Texture;
    class BitmapFont;
    class Shader;
    class Sprite;
    class TileSet;
    class Map;
    namespace assets{
        class Bundle;
        class Texture;
        class Sprite;
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
        AssetReference<BitmapFont> getFont(uint64_t name);
        AssetReference<BitmapFont> getFont(const char* name);
        AssetReference<Shader> getShader(uint64_t name);
        AssetReference<Shader> getShader(const char* name);
        AssetReference<TileSet> getTileSet(uint64_t name);
        AssetReference<TileSet> getTileSet(const char* name);
        AssetReference<Map> getMap(uint64_t name);
        AssetReference<Map> getMap(const char* name);



        void loadBundle(const std::string& name);
        void loadBundle(uint64_t name, const assets::Bundle* bundle);
    };


}

#endif //GEAR_ASSETMANAGER_H
