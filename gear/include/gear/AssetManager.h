//
// Created by felix on 29/02/2020.
//

#ifndef GEAR_ASSETMANAGER_H
#define GEAR_ASSETMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <future>
#include <optional>
#include <variant>
#include <any>


namespace gear {



    template<class T>
    struct AssetEntry {
        std::optional<const T> store {};

        bool pending() {
            return !store.has_value();
        };

        T& get() {
            return *store; //TODO: deal with pending cast
        }

    };


    template<class T>
    struct AssetReference {
        std::shared_ptr<AssetEntry<T>> ptr;

        bool pending() {
            return ptr ? ptr->pending() : false;
        }

        const T& get() const{
            return ptr->store.value(); //TODO: deal with pending case
        }

        const T* operator->() const {
            return &ptr->store.value();
        }
        const T& operator*() const {
            return ptr->store.value();
        }

    };



    class Texture;
    class TextureAtlas;
    class BitmapFont;
    class Shader;
    class Sprite;
    class TileSet;
    class Map;

    class AssetRegistry {
    public:
        AssetRegistry() = default;
        AssetRegistry(const AssetRegistry&) = delete;
        AssetRegistry& operator=(const AssetRegistry&) = delete;

        AssetReference<Texture> getTexture(const std::string& name);
        AssetReference<Sprite> getSprite(const std::string& name);
        AssetReference<BitmapFont> getFont(const std::string& name);
        AssetReference<Shader> getShader(const std::string& name);
        AssetReference<TileSet> getTileSet(const std::string& name);
        AssetReference<Map> getMap(const std::string& map);

        void loadBundle(const std::string& name);
    private:
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<Texture>>> textures;
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<Sprite>>> sprites;
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<BitmapFont>>> fonts;
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<Shader>>> shaders;
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<TileSet>>> tileSets;
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<Map>>> maps;
    };


}

#endif //GEAR_ASSETMANAGER_H
