---
--- Created by felix.
--- DateTime: 22/04/2020 23:04
---

local targets = require("luabuild.targets");
local ninja = require("luabuild.ninja");

local texPacker = targets.executable {
    name = "texture_packer";
    sources = {
        "main.cpp"
    };
    libs = {
        public = {
            tinyxml2_lib,
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
    name = "TEXTURE_PACK";
    command = texPacker.exe.." -o $name $in";
});

function texture_pack(name, input)
    ninja.writeStep({
        rule = "TEXTURE_PACK";
        outputs = CURRENT_DIRECTORY..name..".bin".." "..CURRENT_DIRECTORY..name..".png";
        inputs = "../"..CURRENT_DIRECTORY..input;
        implicit_dependencies = { texPacker.exe };
        variables = {
            name = CURRENT_DIRECTORY..name;
        };
    });
end