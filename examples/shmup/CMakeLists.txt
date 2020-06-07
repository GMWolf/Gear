
add_executable(shmup src/main.cpp src/Collisions.h src/Collisions.cpp)
target_link_libraries(shmup PUBLIC gear)

add_assets(shmup_assets
        FILE assets.bin
        ATLAS
            assets/sprites/Sprites.tsx
        SHADER
            src/shaders/textured.yaml
            src/shaders/font.yaml
            src/shaders/prim.yaml
        FONT
            assets/fonts/defaultFont.yaml
        )

add_dependencies(shmup shmup_assets)