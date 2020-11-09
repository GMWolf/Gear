//
// Created by felix on 29/02/2020.
//

#include <Assets.h>
#include <gear/assets_generated.h>
#include <fstream>
#include <ios>
#include <iostream>


class gear::AssetRegistry::Store {
public:
    std::unordered_map<std::string, std::unique_ptr<char[]>> fileData;
    std::vector<const assets::Bundle*> bundles;

    const assets::AssetEntry* getAssetEntry(uint64_t name) const;
};

const gear::assets::AssetEntry *gear::AssetRegistry::Store::getAssetEntry(uint64_t name) const {
    for(auto& p : bundles) {
        if (p->assets()) {
            if (auto ptr = p->assets()->LookupByKey(name)) {
                return ptr;
            }
        }
    }
    return nullptr;
}


gear::AssetRegistry::AssetRegistry() : store(std::make_unique<Store>())
{}

gear::AssetRegistry::~AssetRegistry() = default;


void gear::AssetRegistry::registerBundleRecursive(const gear::assets::Bundle* bundle) {
    store->bundles.push_back(bundle);
    if (bundle->nestedBundles()) {
        for (auto nestedBundle : *bundle->nestedBundles()) {
            registerBundleRecursive(nestedBundle->bundle_nested_root());
        }
    }
}

void gear::AssetRegistry::patchPointers(const gear::assets::Bundle* bundle) {

    if (bundle->nestedBundles()) {
        for(auto nested : *bundle->nestedBundles()) {
            patchPointers(nested->bundle_nested_root());
        }
    }

    size_t refCount = bundle->externalRefs() ? bundle->externalRefs()->size() : 0;
    for(int i = 0; i < refCount; i++) {
        auto ref = bundle->externalRefs()->GetMutableObject(i);

        auto entry = store->getAssetEntry(ref->ptr());
        if ((uint8_t)entry->asset_type() == ref->type()) {
            ref->mutate_ptr((uint64_t) entry->asset());
        } else {
            ref->mutate_ptr((uint64_t) nullptr);
        }
    }
}

void gear::AssetRegistry::loadBundle(const std::string & fileName) {

    std::ifstream in(fileName, std::ios::binary);
    in.seekg(0, std::ios::end);
    size_t bufferSize = in.tellg();
    in.seekg(0, std::ios::beg);

    auto buffer = std::make_unique<char[]>(bufferSize);
    in.read(buffer.get(), bufferSize);
    auto bundle = gear::assets::GetBundle(buffer.get());

    registerBundleRecursive(bundle);

    patchPointers(bundle);

    store->fileData.insert({fileName, std::move(buffer)});
}

const gear::assets::Texture* gear::AssetRegistry::getTexture(uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_Texture() : nullptr;
}

const gear::assets::Sprite* gear::AssetRegistry::getSprite(uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_Sprite() : nullptr;
}

const gear::assets::BitmapFont* gear::AssetRegistry::getBitmapFont(const uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_BitmapFont() : nullptr;
}

const gear::assets::Shader* gear::AssetRegistry::getShader(const uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_Shader() : nullptr;
}

const gear::assets::TileSet* gear::AssetRegistry::getTileSet(const uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_TileSet() : nullptr;
}

const gear::assets::Map* gear::AssetRegistry::getMap(const uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_Map() : nullptr;
}

const gear::assets::Font *gear::AssetRegistry::getFont(const uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_Font() : nullptr;
}

const gear::assets::Mesh *gear::AssetRegistry::getMesh(uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_Mesh() : nullptr;
}
