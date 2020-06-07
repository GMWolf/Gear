
local targets = require("luabuild.targets");

targets.executable {

    name = "breakout";
    sources = {"src/main.cpp"};
    libs = {
        public = {
            gear_lib
        }
    }

}

--texture_pack("shmup_textures", "assets/sprites/Sprites.tsx");
--shader_pack("src/shaders/font.yaml");
--shader_pack("src/shaders/prim.yaml");
--shader_pack("src/shaders/textured.yaml");
--font_pack("shmup_default_font", "assets/fonts/defaultFont.yaml");
--map_pack("assets/maps/map1.tmx");
--tileset_pack("assets/maps/tile01.tsx");