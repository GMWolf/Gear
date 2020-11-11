add_executable(3dexample src/main.cpp)
target_link_libraries(3dexample PUBLIC gear-core gear-app gear-g3d)

add_subdirectory(assets/models/SciFiHelmet)
add_subdirectory(assets/models/Corset)

add_assets(3dexample_assets
        FILE assets.bin
        TEXTURE assets/Fabric_Boucle.yaml
        SHADER
            assets/shaders/testshd.yaml
            assets/shaders/default.yaml
        )


add_dependencies(3dexample 3dexample_assets SciFiHelmet Corset)