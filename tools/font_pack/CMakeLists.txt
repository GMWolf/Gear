
add_executable(font_packer main.cpp)
target_link_libraries(font_packer PUBLIC stb yaml-cpp bfg_lyra)
target_include_directories(font_packer PUBLIC ../../)

function(add_font_pack name input)
    add_custom_command(
            COMMAND font_packer ARGS -o ${name} ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${name}.bundle ${name}.png
            DEPENDS "${input}" font_packer
            USES_TERMINAL
    )

    add_custom_target(${name}
            DEPENDS "${name}.bundle")
endfunction()