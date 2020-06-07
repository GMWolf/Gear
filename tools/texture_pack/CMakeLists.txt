add_executable(texture_packer main.cpp)
target_link_libraries(texture_packer PUBLIC tinyxml2 stb yaml-cpp bfg_lyra)
target_include_directories(texture_packer PUBLIC ../..)


function(add_atlas input)
    add_custom_command(
            COMMAND texture_packer ARGS -o ${input} ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${input}.bin ${input}.png
            DEPENDS "${input}" texture_packer
            USES_TERMINAL
    )

    set(ADD_ATLAS_OUT_FILES ${input}.bin ${input}.png PARENT_SCOPE)
endfunction()


