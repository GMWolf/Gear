//
// Created by felix on 29/02/2020.
//

#include <gear/AssetManager.h>
#include <generated/assets_generated.h>
#include <fstream>
#include <ios>
#include <gear/Texture.h>
#include <gear/BitmapFont.h>
#include <gear/Shader.h>
#include <gear/map/TileSet.h>
#include <gear/map/TileMap.h>
#include <gear/map/Map.h>
#include <unordered_map>
#include <iostream>
#include <flatbuffers/hash.h>


template<class T>
struct gear::AssetEntry {
    std::optional<const T> store {};

    bool pending();;

    T& get();
};

template<class T>
T &gear::AssetEntry<T>::get() {
    return *store; //TODO: deal with pending cast
}

template<class T>
bool gear::AssetEntry<T>::pending() {
    return !store.has_value();
}

template<class T>
bool gear::AssetReference<T>::pending() const {
    return ptr && ptr->pending();
}

template<class T>
const T &gear::AssetReference<T>::get() const {
    return ptr->store.value(); //TODO: deal with pending case
}

template<class T>
const T *gear::AssetReference<T>::operator->() const {
    return &ptr->store.value();
}

template<class T>
const T &gear::AssetReference<T>::operator*() const {
    return ptr->store.value();
}

template class gear::AssetReference<gear::TileSet>;
template class gear::AssetReference<gear::TileMap>;
template class gear::AssetReference<gear::Map>;

class gear::AssetRegistry::Store {
public:
    template<class T>
    class ResourceStore {
    public:
        std::unordered_map<uint64_t, std::shared_ptr<AssetEntry<T>>> map;
        gear::AssetReference<T> get(uint64_t s);
    };


    ResourceStore<TileSet> tileSets;
    ResourceStore<Map> maps;

    std::unordered_map<std::string, std::unique_ptr<char[]>> fileData;
    std::unordered_map<uint64_t, const assets::Bundle*> bundles;

    const assets::AssetEntry* getAssetEntry(uint64_t name) const;
};

const gear::assets::AssetEntry *gear::AssetRegistry::Store::getAssetEntry(uint64_t name) const {
    for(auto& p : bundles) {
        if (auto ptr = p.second->assets()->LookupByKey(name)) {
            return ptr;
        }
    }
    return nullptr;
}

template<class T>
gear::AssetReference<T> gear::AssetRegistry::Store::ResourceStore<T>::get(const uint64_t s) {
    auto it = map.find(s);
    if (it == map.end()) {
        auto d = std::make_shared<AssetEntry<T>>();
        it = map.insert({s, d}).first;
    }
    return {it->second};
}


gear::AssetRegistry::AssetRegistry() : store(std::make_unique<Store>())
{}

gear::AssetRegistry::~AssetRegistry() = default;

void gear::AssetRegistry::loadBundle(uint64_t name, const gear::assets::Bundle *bundle) {

    store->bundles.insert({name, bundle});

    for(auto asset : *bundle->assets()) {
        auto assetName = asset->name();

        switch(asset->asset_type()) {
            case assets::Asset_NONE:
                break;
            case assets::Asset_Texture: {
                //getTexture(name).ptr->store.emplace(TextureLoader::load(asset->asset_as_Texture(), *this, name.c_str()));
            } break;
            case assets::Asset_Sprite: {
                //getSprite(assetName).ptr->store.emplace(SpriteLoader::load(asset->asset_as_Sprite(), *this));
            } break;
            case assets::Asset_Font: {
                //getFont(assetName).ptr->store.emplace(BitmapFontLoader::load(asset->asset_as_Font(), *this));
            } break;
            case assets::Asset_Shader: {
                //getShader(assetName).ptr->store.emplace(ShaderLoader::load(asset->asset_as_Shader(), *this));
            } break;
            case assets::Asset_TileSet:
                getTileSet(assetName).ptr->store.emplace(loadTileSet(asset->asset_as_TileSet(), *this));
                break;
            case assets::Asset_Map:
                getMap(assetName).ptr->store.emplace(loadMap(asset->asset_as_Map(), *this));
                break;
            case assets::Asset_NestedBundle:
                loadBundle(assetName, asset->asset_as_NestedBundle()->bundle_nested_root());
                break;
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

    loadBundle(flatbuffers::HashFnv1<uint64_t>(fileName.c_str()), bundle);

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

const gear::assets::Font* gear::AssetRegistry::getFont(const uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_Font() : nullptr;
}

const gear::assets::Shader* gear::AssetRegistry::getShader(const uint64_t name) {
    auto entry = store->getAssetEntry(name);
    return entry ? entry->asset_as_Shader() : nullptr;
}

gear::AssetReference<gear::TileSet> gear::AssetRegistry::getTileSet(const uint64_t name) {
    return store->tileSets.get(name);
}

gear::AssetReference<gear::Map> gear::AssetRegistry::getMap(const uint64_t name) {
    return store->maps.get(name);
}

const gear::assets::Texture *gear::AssetRegistry::getTexture(const char* name) {
    return getTexture(flatbuffers::HashFnv1<uint64_t>(name));
}

const gear::assets::Sprite* gear::AssetRegistry::getSprite(const char *name) {
    return getSprite(flatbuffers::HashFnv1<uint64_t>(name));
}

const gear::assets::Font* gear::AssetRegistry::getFont(const char *name) {
    return getFont(flatbuffers::HashFnv1<uint64_t>(name));
}

const gear::assets::Shader* gear::AssetRegistry::getShader(const char *name) {
    return getShader(flatbuffers::HashFnv1<uint64_t>(name));
}

gear::AssetReference<gear::TileSet> gear::AssetRegistry::getTileSet(const char *name) {
    return getTileSet(flatbuffers::HashFnv1<uint64_t>(name));
}

gear::AssetReference<gear::Map> gear::AssetRegistry::getMap(const char *name) {
    return getMap(flatbuffers::HashFnv1<uint64_t>(name));
}


