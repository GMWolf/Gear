
local targets = require("luabuild.targets");
local ninja = require("luabuild.ninja");

local tilesetPacker = targets.executable {
    name = "tileset_pack";
    sources = {
        "tileset_pack.cpp"
    };
    libs = {
        public = {
            tinyxml2_lib, stb_lib, json_lib, yaml_lib
        }
    };
    include_directories = {
        public = {
            "../../"
        }
    }
}

ninja.writeRule({
    name = "TILESET_PACK";
    command = tilesetPacker.exe.." $in $out";
});

function tileset_pack(input)
    ninja.writeStep({
        rule = "TILESET_PACK";
        outputs = CURRENT_DIRECTORY..input..".bin";
        inputs = "../"..CURRENT_DIRECTORY..input;
        implicit_dependencies = { tilesetPacker.exe };
    });
end