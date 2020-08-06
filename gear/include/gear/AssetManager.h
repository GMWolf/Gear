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
    }

    class AssetRegistry {
        class Store;
        std::unique_ptr<Store> store;
    public:
        AssetRegistry();
        AssetRegistry(const AssetRegistry&) = delete;
        AssetRegistry& operator=(const AssetRegistry&) = delete;
        ~AssetRegistry();

        AssetReference<Texture> getTexture(const std::string& name);
        AssetReference<Sprite> getSprite(const std::string& name);
        AssetReference<BitmapFont> getFont(const std::string& name);
        AssetReference<Shader> getShader(const std::string& name);
        AssetReference<TileSet> getTileSet(const std::string& name);
        AssetReference<Map> getMap(const std::string& name);

        void loadBundle(const std::string& name);
        void loadBundle(const assets::Bundle* bundle);
    };


}

#endif //GEAR_ASSETMANAGER_H
