
add_subdirectory(font_pack)
add_subdirectory(shader_pack)
add_subdirectory(texture_pack)



function(add_assets target)

    set(multiValueArgs ATLAS SHADER FONT)
    cmake_parse_arguments(PARSE_ARGV 1 ADD_ASSETS "" "" "${multiValueArgs}")

    foreach(file ${ADD_ASSETS_ATLAS})
        add_atlas(${file})
        list(APPEND ASSETS ${ADD_ATLAS_OUT_FILES})
    endforeach()

    foreach(file ${ADD_ASSETS_SHADER})
        add_shader(${file})
        list(APPEND ASSETS ${ADD_SHADER_OUT_FILES})
    endforeach()

    foreach(file ${ADD_ASSETS_FONT})
        add_font(${file})
        list(APPEND ASSETS ${ADD_FONT_OUT_FILES})
    endforeach()

    add_custom_target(${target} DEPENDS ${ASSETS})
endfunction()