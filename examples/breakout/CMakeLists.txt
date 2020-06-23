add_executable(breakout src/main.cpp)
target_link_libraries(breakout PUBLIC gear)

add_assets(breakout_assets
        FILE assets.bin
        ATLAS
        assets/sprites/sprites.tsx
        SHADER
        assets/shaders/textured.yaml
        )

add_dependencies(breakout breakout_assets)