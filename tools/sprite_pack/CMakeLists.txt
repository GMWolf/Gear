add_executable(sprite_packer main.cpp)
target_link_libraries(sprite_packer PUBLIC gear_tools_texture tinyxml2 stb yaml-cpp bfg_lyra)
target_include_directories(sprite_packer PUBLIC ../..)

function(add_atlas input)
    add_custom_command(
            COMMAND sprite_packer ARGS -o ${input} ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${input}.bin
            DEPENDS "${input}" sprite_packer
            USES_TERMINAL
    )

    set(ADD_ATLAS_OUT_FILES ${input}.bin ${input}.png PARENT_SCOPE)
    set(ADD_ATLAS_BUNDLES ${input}.bin PARENT_SCOPE)
endfunction()


