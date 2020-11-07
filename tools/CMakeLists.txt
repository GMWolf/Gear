add_subdirectory(texture)

add_subdirectory(font_pack)
add_subdirectory(shader_pack)
add_subdirectory(sprite_pack)
add_subdirectory(texture_pack)
add_subdirectory(tileset_pack)
add_subdirectory(map_pack)
add_subdirectory(bundler)


function(add_assets target)
    set(oneValueArgs FILE)
    set(multiValueArgs TEXTURE ATLAS SHADER FONT TILESET MAP)
    cmake_parse_arguments(PARSE_ARGV 1 ADD_ASSETS "" "${oneValueArgs}" "${multiValueArgs}")

    foreach(file ${ADD_ASSETS_TEXTURE})
        add_texture(${file})
        list(APPEND ASSETS ${ADD_TEXTURE_OUT_FILES})
        list(APPEND BUNDLES ${ADD_TEXTURE_BUNDLES})
    endforeach()

    foreach(file ${ADD_ASSETS_ATLAS})
        add_atlas(${file})
        list(APPEND ASSETS ${ADD_ATLAS_OUT_FILES})
        list(APPEND BUNDLES ${ADD_ATLAS_BUNDLES})
    endforeach()

    foreach(file ${ADD_ASSETS_SHADER})
        add_shader(${file})
        list(APPEND ASSETS ${ADD_SHADER_OUT_FILES})
        list(APPEND BUNDLES ${ADD_SHADER_BUNDLES})
    endforeach()

    foreach(file ${ADD_ASSETS_FONT})
        add_font(${file})
        list(APPEND ASSETS ${ADD_FONT_OUT_FILES})
        list(APPEND BUNDLES ${ADD_FONT_BUNDLES})
    endforeach()

    foreach(file ${ADD_ASSETS_TILESET})
        add_tileset_pack(${file})
        list(APPEND ASSETS ${ADD_TILESET_OUT_FILES})
        list(APPEND BUNDLES ${ADD_TILESET_BUNDLES})
    endforeach()

    foreach(file ${ADD_ASSETS_MAP})
        add_map(${file})
        list(APPEND ASSETS ${ADD_MAP_OUT_FILES})
        list(APPEND BUNDLES ${ADD_MAP_BUNDLES})
    endforeach()

    add_custom_target(${target}_ASSETS DEPENDS ${ASSETS})

    add_bundle(${ADD_ASSETS_FILE} ${target} ${BUNDLES})

    add_custom_target(${target} DEPENDS ${ADD_ASSETS_FILE})
endfunction()