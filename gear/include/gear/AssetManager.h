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

namespace gear {

    struct AssetEntry {
        std::shared_ptr<void> object;
    };

    class AssetLoader {
    public:
        virtual AssetEntry load(const std::string& name) = 0;
        virtual ~AssetLoader() = default;
    };

    class AssetManager {
    public:
        std::shared_ptr<void> get(const std::string& name) const;
        template<class T> std::shared_ptr<const T> get_as(const std::string& name) const;

        void load(const std::string& name, AssetLoader& assetLoader);

        template<class T>
        void load(const std::string& name);

        template<class T, class L>
        void setLoader();
    private:
        std::unordered_map<std::string, AssetEntry> assets;
        std::unordered_map<std::type_index, std::unique_ptr<AssetLoader>> loaders;
    };


    template<class T>
    std::shared_ptr<const T> AssetManager::get_as(const std::string &name) const{
        return std::static_pointer_cast<T>(get(name));
    }

    template<class T>
    void AssetManager::load(const std::string &name) {
        load(name, *loaders[typeid(T)]);
    }

    template<class T, class L>
    void AssetManager::setLoader() {
        loaders[typeid(T)] = std::make_unique<L>();
    }


}

#endif //GEAR_ASSETMANAGER_H
