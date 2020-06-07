
add_executable(shader_packer shader_packer.cpp)
target_link_libraries(shader_packer PUBLIC yaml-cpp bfg_lyra)
target_include_directories(shader_packer PUBLIC ../../)

function(add_shader_pack name input)
    add_custom_command(
            COMMAND shader_packer ARGS -o ${input}.bin ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${input}.bin
            DEPENDS "${input}" shader_packer
            USES_TERMINAL
    )

    add_custom_target(${name}
            DEPENDS "${input}.bin")
endfunction()