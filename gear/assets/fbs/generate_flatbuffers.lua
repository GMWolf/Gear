---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by felix.
--- DateTime: 26/04/2020 00:11
---


local schemas = {
    "common.fbs",
    "sprite.fbs",
    "bitmapFont.fbs",
    "font.fbs",
    "map.fbs",
    "shader.fbs",
    "assets.fbs",
    "tileset.fbs",
    "texture.fbs",
    "mesh.fbs"
}


os.execute("flatc --cpp --gen-mutable --cpp-std 'c++17' -o generated/gear "..table.concat(schemas, " "));