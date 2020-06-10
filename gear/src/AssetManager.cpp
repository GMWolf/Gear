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
bool gear::AssetReference<T>::pending() {
    return ptr ? ptr->pending() : false;
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



template class gear::AssetReference<gear::Texture>;
template class gear::AssetReference<gear::Sprite>;
template class gear::AssetReference<gear::Shader>;
template class gear::AssetReference<gear::BitmapFont>;
template class gear::AssetReference<gear::TileSet>;
template class gear::AssetReference<gear::TileMap>;

class gear::AssetRegistry::Impl {
public:
    template<class T>
    class Store {
    public:
        std::unordered_map<std::string, std::shared_ptr<AssetEntry<T>>> map;
        gear::AssetReference<T> get(const std::string& s);
    };


    Store<Texture> textures;
    Store<Sprite> sprites;
    Store<BitmapFont> fonts;
    Store<Shader> shaders;
    Store<TileSet> tileSets;
    Store<Map> maps;
};

template<class T>
gear::AssetReference<T> gear::AssetRegistry::Impl::Store<T>::get(const std::string &s) {
    auto it = map.find(s);
    if (it == map.end()) {
        auto d = std::make_shared<AssetEntry<T>>();
        it = map.insert({s, d}).first;
    }
    return {it->second};
}


gear::AssetRegistry::AssetRegistry() : impl(std::make_unique<Impl>())
{}

gear::AssetRegistry::~AssetRegistry() = default;

void gear::AssetRegistry::loadBundle(const gear::assets::Bundle *bundle) {
    for(auto asset : *bundle->assets()) {
        auto name = asset->name()->str();
        switch(asset->asset_type()) {
            case assets::Asset_NONE:
                break;
            case assets::Asset_Texture: {
                getTexture(name).ptr->store.emplace(TextureLoader::load(asset->asset_as_Texture(), *this));
            } break;
            case assets::Asset_Sprite: {
                getSprite(name).ptr->store.emplace(SpriteLoader::load(asset->asset_as_Sprite(), *this));
            } break;
            case assets::Asset_Font: {
                getFont(name).ptr->store.emplace(BitmapFontLoader::load(asset->asset_as_Font(), *this));
            } break;
            case assets::Asset_Shader: {
                getShader(name).ptr->store.emplace(ShaderLoader::load(asset->asset_as_Shader(), *this));
            } break;
            case assets::Asset_TileSet:
                //getTileSet(name).ptr->store.emplace()
                break;
            case assets::Asset_Map:
                break;
            case assets::Asset_NestedBundle:
                loadBundle(asset->asset_as_NestedBundle()->bundle_nested_root());
                break;
        }
    }
}


void gear::AssetRegistry::loadBundle(const std::string & fileName) {

    std::ifstream in(fileName, std::ios::binary);
    in.seekg(0, std::ios::end);
    size_t bufferSize = in.tellg();
    in.seekg(0, std::ios::beg);
    auto buffer = (char*)malloc(bufferSize);
    in.read(buffer, bufferSize);

    auto bundle = gear::assets::GetBundle(buffer);

    loadBundle(bundle);

    free(buffer);
}

gear::AssetReference<gear::Texture> gear::AssetRegistry::getTexture(const std::string &name) {
    return impl->textures.get(name);
}

gear::AssetReference<gear::Sprite> gear::AssetRegistry::getSprite(const std::string &name) {
    return impl->sprites.get(name);
}

gear::AssetReference<gear::BitmapFont> gear::AssetRegistry::getFont(const std::string &name) {
    return impl->fonts.get(name);
}

gear::AssetReference<gear::Shader> gear::AssetRegistry::getShader(const std::string &name) {
    return impl->shaders.get(name);
}

gear::AssetReference<gear::TileSet> gear::AssetRegistry::getTileSet(const std::string &name) {
    return impl->tileSets.get(name);
}

gear::AssetReference<gear::Map> gear::AssetRegistry::getMap(const std::string &name) {
    return impl->maps.get(name);
}


