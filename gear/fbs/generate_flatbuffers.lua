---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by felix.
--- DateTime: 26/04/2020 00:11
---


local schemas = {
    "texture_atlas.fbs"
}


os.execute("flatc --cpp -o generated "..table.concat(schemas, " "));