
set(GEAR_ECS_SOURCES
        include/gear/ECS/Core.h
        src/ECS/Core.cpp
        include/gear/ECS/Archetype.h
        src/ECS/Archetype.cpp
        include/gear/ECS/CommandBuffer.h
        src/ECS/CommandBuffer.cpp
        include/gear/ECS/Component.h
        src/ECS/Component.cpp
        include/gear/ECS/Config.h
        include/gear/ECS/ECS.h
        include/gear/ECS/ChunkView.h
        include/gear/ECS/System.h
        include/gear/ECS/Prefab.h
        src/ECS/Prefab.cpp
        )



add_library(gear
        include/gear/Application.h
        src/Application.cpp
        include/gear/SpriteBatch.h
        src/SpriteBatch.cpp
        include/gear/Texture.h
        src/Texture.cpp
        include/gear/Shader.h
        src/Shader.cpp
        include/gear/CollisionDetection.h
        src/CollisionDetection.cpp
        include/gear/View.h
        src/View.cpp
        include/gear/BitmapFont.h
        src/BitmapFont.cpp
        include/gear/AssetManager.h
        src/AssetManager.cpp
        include/gear/DebugUI.h
        src/DebugUI.cpp
        include/gear/Allocators.h
        src/Allocators.cpp
        include/gear/RenderSystem.h
        src/RenderSystem.cpp
        include/gear/map/TileSet.h
        src/map/TileSet.cpp
        include/gear/map/TileMap.h
        src/map/TileMap.cpp
        include/gear/map/TilemapSystem.h
        src/map/TilemapSystem.cpp
        include/gear/map/Map.h
        src/map/Map.cpp
        include/gear/PrimDraw.h
        src/PrimDraw.cpp
        )

target_sources(gear PRIVATE ${GEAR_ECS_SOURCES})

target_include_directories(gear PUBLIC include fbs)

target_link_libraries(gear PUBLIC glad imgui imgui_glfw imgui_ogl3 glm glfw flatbuffers lz4_static)