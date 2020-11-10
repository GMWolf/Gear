
add_executable(shader_packer shader_packer.cpp)
target_link_libraries(shader_packer PUBLIC yaml-cpp bfg_lyra gear-assets-fbs shaderc spirv-cross-cpp)
target_include_directories(shader_packer PUBLIC ../../)

function(add_shader input)
    add_custom_command(
            COMMAND shader_packer ARGS -o ${input}.bin ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${input}.bin
            DEPENDS "${input}" shader_packer
            DEPFILE ${input}.bin.d
            USES_TERMINAL
    )

    set(ADD_SHADER_OUT_FILES ${input}.bin PARENT_SCOPE)
    set(ADD_SHADER_BUNDLES ${input}.bin PARENT_SCOPE)
endfunction()

