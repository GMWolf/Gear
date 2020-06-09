
add_executable(font_packer main.cpp)
target_link_libraries(font_packer PUBLIC stb yaml-cpp bfg_lyra lz4_static)
target_include_directories(font_packer PUBLIC ../../)

function(add_font input)
    add_custom_command(
            COMMAND font_packer ARGS -o ${input} ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${input}.bin ${input}.png
            DEPENDS "${input}" font_packer
            USES_TERMINAL
    )

    set(ADD_FONT_OUT_FILES ${input}.bin ${input}.png PARENT_SCOPE)
    set(ADD_FONT_BUNDLES ${input}.bin PARENT_SCOPE)
endfunction()