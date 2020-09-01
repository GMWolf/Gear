
set(GEAR_G2D_PUBLIC
        include/gear/g2d/g2d.h
        include/gear/g2d/PrimDraw.h
        include/gear/g2d/RenderSystem.h
        include/gear/g2d/SpriteBatch.h
        include/gear/g2d/Shader.h
        include/gear/g2d/Text.h
        include/gear/g2d/TilemapSystem.h
        include/gear/g2d/DebugUI.h
        )

set(GEAR_G2D_HEADERS
        src/Texture.h
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
        src/G2DInstance.cpp include/gear/g2d/Sprite.h src/Sprite.cpp src/G2DInstance.h)

add_library(gear-g2d ${GEAR_G2D_PUBLIC} ${GEAR_G2D_HEADERS} ${GEAR_G2D_SOURCES})

target_link_libraries(gear-g2d
        PRIVATE imgui imgui_glfw imgui_ogl3 libzstd_static)
target_link_libraries(gear-g2d
        PUBLIC gear-core gear-assets glad
        )
target_include_directories(gear-g2d
        PUBLIC include
        PRIVATE include/gear/g2d)
