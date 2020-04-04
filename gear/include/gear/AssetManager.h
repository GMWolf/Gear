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

    class AssetRegistry;

    template<class T>
    class AssetLoader {
    public:
        virtual T load(const std::string& name, AssetRegistry& registry) = 0;
        virtual ~AssetLoader() = default;
    };

    class AssetRegistry {
    public:
        template<class T> AssetReference<T> get(const std::string& name) ;

        template<class T>
        void load(const std::string& name, AssetLoader<T>& assetLoader);

        template<class T>
        void load(const std::string& name);

        template<class T, class L>
        void setLoader(L&& loader);
    private:
        std::unordered_map<std::string, std::shared_ptr<void>> assets;
        std::unordered_map<std::type_index, std::shared_ptr<void>> loaders;
    };


    template<class T>
    AssetReference<T> AssetRegistry::get(const std::string &name) {
        auto it = assets.find(name);
        if (it == assets.end()) {
            std::shared_ptr<void> d = std::make_shared<AssetEntry<T>>();
            it = assets.insert({name, d}).first;
        }
        return {std::static_pointer_cast<AssetEntry<T>>(it->second)};
    }

    template<class T>
    void gear::AssetRegistry::load(const std::string &name, AssetLoader<T> &assetLoader) {
        get<T>(name).ptr->store.emplace(assetLoader.load(name, *this));
    }

    template<class T>
    void AssetRegistry::load(const std::string &name) {
        AssetLoader<T>& loader = *static_cast<AssetLoader<T>*>(loaders[typeid(T)].get());
        load(name, loader);
    }

    template<class T, class L>
    void AssetRegistry::setLoader(L&& loader) {
        loaders[typeid(T)] = std::make_shared<L>(std::forward<L>(loader));
    }


}

#endif //GEAR_ASSETMANAGER_H
