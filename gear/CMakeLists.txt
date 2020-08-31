


add_subdirectory(utils)
add_subdirectory(ecs)
add_subdirectory(app)

add_library(gear
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
        include/gear/PrimDraw.h
        src/PrimDraw.cpp
        include/gear/Job.h
        src/Job.cpp)

target_include_directories(gear PUBLIC include fbs)

target_link_libraries(gear PUBLIC glad imgui imgui_glfw imgui_ogl3 glm flatbuffers libzstd_static Boost::context)
target_link_libraries(gear PUBLIC gear-utils gear-ecs gear-app)