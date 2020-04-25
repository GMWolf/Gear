local target = require("luabuild.targets");

local gear_source = {
    "src/Application.cpp" ,
    "src/SpriteBatch.cpp",
    "src/Texture.cpp",
    "src/Shader.cpp",
    "src/TextureAtlas.cpp",
    "src/CollisionDetection.cpp",
    "src/View.cpp",
    "src/ECS/Core.cpp",
    "src/ECS/Archetype.cpp",
    "src/ECS/Component.cpp",
    "src/BitmapFont.cpp",
    "src/AssetManager.cpp",
    "src/DebugUI.cpp",
    "src/Allocators.cpp",
    "src/ECS/CommandBuffer.cpp",
    "src/RenderSystem.cpp",
    "src/map/TileSet.cpp",
    "src/map/TileMap.cpp",
    "src/map/TilemapSystem.cpp",
}


local l = flatc("fbs/test.fbs");

return target.library {
    name = "gear";
    sources = gear_source;
    libs = {
        public = {glad_lib, imgui_lib, stb_lib, json_lib, tinyxml2_lib, base64_lib, glfw_lib,
                  glm_lib, l};
    };
    include_directories = {
        public = {"include"};
    };
}

