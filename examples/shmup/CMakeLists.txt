
add_executable(shmup src/main.cpp src/Collisions.h src/Collisions.cpp)
target_link_libraries(shmup PUBLIC gear-core gear-app gear-g2d)

add_assets(shmup_assets
        FILE assets.bin
        ATLAS
            assets/sprites/Sprites.tsx
        TILESET
            assets/maps/tile01.tsx
        SHADER
            src/shaders/textured.yaml
            src/shaders/font.yaml
            src/shaders/prim.yaml
        FONT
            assets/fonts/defaultFont.yaml
        MAP
            assets/maps/map1.tmx
        )

add_dependencies(shmup shmup_assets)