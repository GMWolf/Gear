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

    class AssetRegistry {
    public:
        AssetRegistry() = default;
        AssetRegistry(const AssetRegistry&) = delete;
        AssetRegistry& operator=(const AssetRegistry&) = delete;

        AssetReference<Texture> getTexture(const std::string& name);
        AssetReference<TextureAtlas> getAtlas(const std::string& name);
        AssetReference<BitmapFont> getFont(const std::string& name);
        AssetReference<Shader> getShader(const std::string& name);

        void loadBundle(const std::string& name);
    private:
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<Texture>>> textures;
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<TextureAtlas>>> atlases;
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<BitmapFont>>> fonts;
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<Shader>>> shaders;
    };




}

#endif //GEAR_ASSETMANAGER_H
