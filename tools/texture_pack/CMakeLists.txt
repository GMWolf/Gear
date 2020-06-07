add_executable(texture_packer main.cpp)
target_link_libraries(texture_packer PUBLIC tinyxml2 stb yaml-cpp bfg_lyra)
target_include_directories(texture_packer PUBLIC ../..)


function(add_texture_pack name input)

    add_custom_command(
            COMMAND texture_packer ARGS -o ${name} ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${name}.bin ${name}.png
            DEPENDS "${input}" texture_packer
            USES_TERMINAL
    )

    add_custom_target(${name}
            DEPENDS "${name}.bin" "${name}.png")
endfunction()