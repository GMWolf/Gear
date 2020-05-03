//
// Created by felix on 29/02/2020.
//

#include <gear/AssetManager.h>
#include <generated/assets_generated.h>
#include <fstream>
#include <ios>
#include <gear/Texture.h>
#include <gear/TextureAtlas.h>
#include <gear/BitmapFont.h>
#include <gear/Shader.h>

void gear::AssetRegistry::loadBundle(const std::string & fileName) {

    std::ifstream in(fileName, std::ios::binary);
    in.seekg(0, std::ios::end);
    size_t bufferSize = in.tellg();
    in.seekg(0, std::ios::beg);
    auto buffer = (char*)malloc(bufferSize);
    in.read(buffer, bufferSize);

    auto bundleDef = gear::assets::GetBundle(buffer);

    for(auto asset : *bundleDef->assets()) {
        auto name = asset->name()->str();
        switch(asset->asset_type()) {
            case assets::Asset_NONE:
                break;
            case assets::Asset_texture: {
                auto path = asset->asset_as_texture()->str();
                getTexture(name).ptr->store.emplace(TextureLoader::load(path, *this));
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
        }
    }

    free(buffer);
}

gear::AssetReference<gear::Texture> gear::AssetRegistry::getTexture(const std::string &name) {
    auto it = textures.find(name);
    if (it == textures.end()) {
        auto d = std::make_shared<AssetEntry<Texture>>();
        it = textures.insert({name, d}).first;
    }
    return {it->second};
}

gear::AssetReference<gear::Sprite> gear::AssetRegistry::getSprite(const std::string &name) {
    auto it = sprites.find(name);
    if (it == sprites.end()) {
        auto d = std::make_shared<AssetEntry<Sprite>>();
        it = sprites.insert({name, d}).first;
    }
    return {it->second};
}

gear::AssetReference<gear::BitmapFont> gear::AssetRegistry::getFont(const std::string &name) {
    auto it = fonts.find(name);
    if (it == fonts.end()) {
        auto d = std::make_shared<AssetEntry<BitmapFont>>();
        it = fonts.insert({name, d}).first;
    }
    return {it->second};
}

gear::AssetReference<gear::Shader> gear::AssetRegistry::getShader(const std::string &name) {
    auto it = shaders.find(name);
    if (it == shaders.end()) {
        auto d = std::make_shared<AssetEntry<Shader>>();
        it = shaders.insert({name, d}).first;
    }
    return {it->second};
}
