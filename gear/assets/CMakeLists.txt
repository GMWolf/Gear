

add_subdirectory(fbs)


set(GEAR_ASSETS_HEADERS
        include/gear/Assets.h
        include/gear/TileSet.h
        include/gear/TileMap.h
        )

set(GEAR_ASSETS_SOURCE
        src/Assets.cpp
        src/TileSet.cpp
        src/TileMap.cpp
        )

add_library(gear-assets ${GEAR_ASSETS_HEADERS} ${GEAR_ASSETS_SOURCE} ${GEAR_ASSETS_GENERATED})
target_link_libraries(gear-assets PUBLIC gear-assets-fbs)
target_include_directories(gear-assets
        PUBLIC include
        PRIVATE include/gear)