---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by felix.
--- DateTime: 29/04/2020 23:26
---


local targets = require("luabuild.targets");
local ninja = require("luabuild.ninja");

local mapPacker = targets.executable {
    name = "map_pack";
    sources = {
        "main.cpp"
    };
    libs = {
        public = {
            tinyxml2_lib, base64_lib,
            stb_lib, json_lib, yaml_lib, lyra_lib
        }
    };
    include_directories = {
        public = {
            "../../"
        }
    }
}

ninja.writeRule({
    name = "MAP_PACK";
    command = mapPacker.exe.." -o $out $in";
});

function map_pack(input)
    ninja.writeStep({
        rule = "MAP_PACK";
        outputs = CURRENT_DIRECTORY..input..".bin";
        inputs = "../"..CURRENT_DIRECTORY..input;
        implicit_dependencies = { mapPacker.exe };
    });
end