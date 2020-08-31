set(GEAR_ASSETS_GENERATED
        generated/gear/assets_generated.h
        generated/gear/common_generated.h
        generated/gear/font_generated.h
        generated/gear/map_generated.h
        generated/gear/shader_generated.h
        generated/gear/sprite_generated.h
        generated/gear/texture_generated.h
        generated/gear/tileset_generated.h
        )

add_library(gear-assets-fbs INTERFACE)
target_sources(gear-assets-fbs INTERFACE ${GEAR_ASSETS_GENERATED})
target_include_directories(gear-assets-fbs INTERFACE generated)
target_link_libraries(gear-assets-fbs INTERFACE flatbuffers)
