add_executable(texture_packer main.cpp)
target_link_libraries(texture_packer PUBLIC gear_tools_texture stb yaml-cpp bfg_lyra)
target_include_directories(texture_packer PUBLIC ../..)

function(add_texture input)
    add_custom_command(
            COMMAND texture_packer ARGS -o ${input}.bin ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${input}.bin
            DEPENDS "${input}" texture_packer
            USES_TERMINAL
    )

    set(ADD_TEXTURE_OUT_FILES ${input}.bin PARENT_SCOPE)
    set(ADD_TEXTURE_BUNDLES ${input}.bin PARENT_SCOPE)
endfunction()


