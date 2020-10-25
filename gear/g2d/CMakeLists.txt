
set(GEAR_G2D_PUBLIC
        include/gear/g2d/g2d.h
        include/gear/g2d/RenderSystem.h
        include/gear/g2d/Text.h
        include/gear/g2d/TilemapSystem.h
        include/gear/g2d/DebugUI.h
        include/gear/g2d/Sprite.h
        )

set(GEAR_G2D_HEADERS
        src/Texture.h
        src/PrimDraw.h
        src/FontCache.h
        src/SpriteBatch.h
        src/Shader.h
        )

set(GEAR_G2D_SOURCES
        src/PrimDraw.cpp
        src/RenderSystem.cpp
        src/Shader.cpp
        src/SpriteBatch.cpp
        src/Text.cpp
        src/Texture.cpp
        src/TilemapSystem.cpp
        src/DebugUI.cpp
        src/G2DInstance.cpp
        src/Sprite.cpp
        src/FontCache.h
        src/FontCache.cpp)

add_library(gear-g2d ${GEAR_G2D_PUBLIC} ${GEAR_G2D_HEADERS} ${GEAR_G2D_SOURCES})

target_link_libraries(gear-g2d
        PRIVATE imgui imgui_glfw imgui_ogl3 libzstd_static freetype)
target_link_libraries(gear-g2d
        PUBLIC gear-core gear-assets glad gear-gapi
        )
target_include_directories(gear-g2d
        PUBLIC include
        PRIVATE include/gear/g2d)
