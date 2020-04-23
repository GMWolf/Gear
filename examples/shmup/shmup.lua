---
--- Created by felix.
--- DateTime: 20/04/2020 23:33
---

local targets = require("luabuild.targets");

targets.executable {

    name = "shmup";
    sources = {"src/main.cpp", "src/Collisions.cpp"};
    libs = {
        public = {
            gear_lib
        }
    }

}

texture_pack("shmup_textures", "assets/sprites/sprites.yaml");
shader_pack("simple_textured", "src/shaders/simple_textured.vert", "src/shaders/simple_textured.frag");
shader_pack("shd_font", "src/shaders/simple_textured.vert", "src/shaders/font.frag")
font_pack("shmup_default_font", "assets/fonts/defaultFont.yaml");