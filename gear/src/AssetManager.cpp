//
// Created by felix on 29/02/2020.
//

#include <gear/AssetManager.h>

std::shared_ptr<void> gear::AssetManager::get(const std::string &name) {
    auto it = assets.find(name);
    if (it != assets.end()){
        return it->second.object;
    }
    return {};
}


void gear::AssetManager::load(const std::string &name, AssetLoader &assetLoader) {
    assets[name] = assetLoader.load(name);
}

